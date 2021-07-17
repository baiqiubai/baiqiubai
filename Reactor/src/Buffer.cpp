
#include "Buffer.h"
#include "LogStream.h"
#include "Logger.h"

#include <assert.h>
#include <sys/ioctl.h>
#include <cstring>

#define BUFFER_CHAIN_SIZE sizeof(BufferChain)
#define BUFFER_MAX_READ 4096
#define NUM_READ_IOVEC 4
#define NUM_WRITE_IOVEC 64

using namespace base;
namespace net {
namespace detail {

size_t kDefaultAllocSize = 512;
BufferChain *createNewBufferChain(size_t totalSize, size_t misalign)
{
    size_t toalloc = kDefaultAllocSize;

    while (toalloc < totalSize) toalloc *= 2;

    BufferChain *chain = new BufferChain(toalloc, misalign);

    return chain;
}

size_t getSockfdBytes(int sockfd)
{
    size_t n = BUFFER_MAX_READ;
    if (::ioctl(sockfd, FIONREAD, &n) < 0) LOG_SYSFATAL << "ioctl error";
    return n;
}

}  // namespace detail

void Buffer::deleteAllChain()
{
    BufferChain *chain = first_;
    BufferChain *next = NULL;
    for (; chain; chain = next) {
        next = chain->next_;
        delete chain;
    }
}

bool BufferChain::shouldRealign(size_t dataSize)
{
    return bufferSize_ - off_ > dataSize;
}

void BufferChain::Realign()
{
    //移动到开头
    ::memmove(buffer_, buffer_ + misalign_, off_);
    misalign_ = 0;
}
void Buffer::foreachWithDataChain()
{
    BufferChain *chain = first_;
    int cnt = 1;
    for (; chain != NULL; chain = chain->next_) {
        printf("cnt %d Buffer Size %ld,Off %ld\n", cnt, chain->bufferSize(),
               chain->off());
        cnt++;
    }
}

int Buffer::afterChainisEmpty(BufferChain *chain)
{
    for (; chain; chain = chain->next_)
        if (chain->off()) return 0;
    return 1;
}
void Buffer::deleteAfterChain(BufferChain *chain)
{
    for (; chain != NULL; chain = chain->next_) delete chain;
}
void Buffer::bufferInsertChain(BufferChain *chain)
{
    BufferChain **ch = lastWithData_;

    while ((*ch) && (*ch)->off())  //找到off为0的
        ch = &(*ch)->next_;

    if (*ch == NULL) {  //没有找到
        *ch = chain;    //插入
        if (chain->off()) lastWithData_ = ch;
    }
    else {
        assert(afterChainisEmpty(*ch));
        deleteAfterChain(*ch);
        *ch = chain;
        if (chain->off()) lastWithData_ = ch;
    }
    last_ = chain;
}
void Buffer::bufferAdd(size_t dataSize, const char *data)
{
    //添加新的chain位于Buffer中 将data中dataSize字节放进Buffer中
    //
    BufferChain *chain;

    if (*lastWithData_ == NULL) {
        chain = last_;
    }
    else {
        chain = *lastWithData_;
    }

    if (chain == NULL) {
        chain = detail::createNewBufferChain(dataSize);
        bufferInsertChain(chain);
    }

    size_t remain = chain->bufferSize() - chain->off() - chain->misalign();

    if (remain > dataSize) {  //完全放的下

        ::memcpy(chain->buffer_ + chain->misalign_ + chain->off_, data,
                 dataSize);
        chain->off_ += dataSize;

        totalSize_ += dataSize;
    }
    else if (chain->shouldRealign(dataSize)) {
        chain->Realign();
        ::memcpy(chain->buffer_ + chain->off_, data, dataSize);

        chain->off_ += dataSize;
        totalSize_ += dataSize;
    }
    else {  //即使调整也放不下

        ::memcpy(chain->buffer_ + chain->misalign_ + chain->off_, data,
                 remain);  //先给最后一个放满
        chain->off_ += remain;

        totalSize_ += remain;

        data += remain;
        remain = dataSize - remain;

        BufferChain *newChain = detail::createNewBufferChain(remain);
        ::memcpy(newChain->misalign_ + newChain->off_ + newChain->buffer_, data,
                 remain);
        bufferInsertChain(newChain);
        newChain->off_ += remain;
        totalSize_ += remain;
    }
}

int Buffer::bufferSetUpVecs(size_t n, struct iovec *vecs, int numIovec)
{
    int sofar = 0;
    int i = 0;
    BufferChain *chain = *lastWithData_;

    assert(chain != NULL);
    for (; sofar < static_cast<int>(n) && i < numIovec; ++i) {
        size_t chainSpace =
            chain->bufferSize() - chain->off() - chain->misalign();
        if (n - sofar < chainSpace) chainSpace = n - sofar;
        vecs[i].iov_base = chain->buffer_ + chain->misalign_;
        vecs[i].iov_len = chainSpace;
        chain = chain->next_;
        sofar += chainSpace;
    }

    return i;
}
void Buffer::bufferExpandFast(size_t n, int IovecNum)
{
    BufferChain *chain = last_;

    if (chain == NULL) {
        chain = detail::createNewBufferChain(n);
        bufferInsertChain(chain);
        return;
    }

    size_t used = 0;  //使用了几个chain

    size_t avail = 0;

    size_t remain = n;
    for (chain = *lastWithData_; chain; chain = chain->next_) {
        size_t chainSpace =
            chain->bufferSize() - chain->misalign() - chain->off();
        if (chain->off()) {
            if (chainSpace) {
                avail += chainSpace;  //可获得的空间
                ++used;
            }
        }
        else {
            chain->misalign_ = 0;
            avail += chain->bufferSize();
            ++used;
        }
        if (avail >= n) return;  //完成
        if (used == n) break;
    }
    if (used < n) {  //不够n个chain 插入一个更大的chain

        BufferChain *newChain = detail::createNewBufferChain(remain - avail);

        last_->next_ = newChain;
        last_ = newChain;
    }
    else {  //
        int removeAll = 0;

        chain = *lastWithData_;
        if (!chain->off()) {  //全部没有数据 删除所有chain

            removeAll = 1;
            avail = 0;
        }
        else {
            avail = chain->bufferSize() - chain->misalign() - chain->off();
            chain = chain->next_;
        }
        for (; chain; chain = chain->next_) {
            assert(chain->off() == 0);
            delete chain;
        }

        BufferChain *newChain = detail::createNewBufferChain(remain - avail);
        // newChain替换 要么替换所有的chain 要么连接在一个有数据的chain后

        if (removeAll) {
            first_ = last_ = newChain;
            lastWithData_ = &first_;
        }
        else {
            (*lastWithData_)->next_ = newChain;
            last_ = newChain;
        }
    }

    return;
}

void Buffer::bufferRead(char *buf, size_t datalen)
{
    assert(datalen >= 0);
    if (bufferCopyout(buf, datalen) == -1) {
        LOG_ERROR << "bufferCopyout error";
        return;
    }
    bufferDrain(datalen);
}
ssize_t Buffer::writeIovec(int sockfd, size_t howmuch)
{
    size_t remain = howmuch;

    int i = 0;
    BufferChain *chain = first_;

    struct iovec vec[NUM_WRITE_IOVEC];
    while (chain != NULL && i < NUM_WRITE_IOVEC && remain) {
        size_t ChainSpace =
            chain->bufferSize() - chain->off() - chain->misalign();

        vec[i].iov_base = chain->buffer_ + chain->misalign_ + chain->off_;
        if (remain >= ChainSpace) {
            vec[i++].iov_len = ChainSpace;
            remain -= ChainSpace;
        }
        else {
            vec[i++].iov_len = remain;
            break;
        }
        chain = chain->next_;
    }
    ssize_t result = ::writev(sockfd, vec, i);
    return result;
}
int Buffer::bufferCopyout(char *dataOut, size_t len)
{
    if (len >= totalSize_) len = totalSize_;  //拷贝能拷贝的所有数据

    size_t remain = len;

    BufferChain *chain = first_;
    if (chain == NULL) {
        return -1;
    }
    else {
        while (chain && remain >= chain->off()) {
            ::memcpy(dataOut, chain->buffer_ + chain->misalign_, chain->off_);
            remain -= chain->off();
            dataOut += chain->off();
            chain = chain->next_;
        }
        if (remain) {  //说明len < totalSize 此时chain不为NULL

            ::memcpy(dataOut, chain->buffer_ + chain->misalign_, remain);
        }
    }
    return 0;
}

//从buffer中删除前datalen个字节
void Buffer::bufferDrain(size_t datalen)
{
    if (datalen >= totalSize_) {  //全部删除
        BufferChain *next;
        for (BufferChain *chain = first_; chain != NULL; chain = next) {
            next = chain->next_;
            delete chain;
        }
        totalSize_ = 0;
        first_ = last_ = NULL;
        lastWithData_ = &first_;
    }
    else {
        size_t remain = datalen;
        totalSize_ -= remain;
        BufferChain *chain = first_;
        BufferChain *next = NULL;
        for (; remain >= chain->off_; chain = next) {
            next = chain->next_;
            remain -= chain->off_;

            if (chain == *lastWithData_)  //最后一个有数据的chain
                lastWithData_ = &first_;

            if (&chain->next_ == lastWithData_) {  //倒数第二个有数据的chain
                lastWithData_ = &first_;
            }

            delete chain;
        }
        first_ = chain;
        if (remain) {  //利用预留空间完成逻辑上删除
            chain->misalign_ += remain;
            chain->off_ -= remain;
        }
    }
}

size_t Buffer::bufferRead(int sockfd)
{
    size_t n;

    size_t result = -1;
    n = detail::getSockfdBytes(sockfd);
    struct iovec vecs[NUM_READ_IOVEC];

    bufferExpandFast(n, NUM_READ_IOVEC);  //给NUM_READ_IOVEC个chain预留空间

    int nvecs = bufferSetUpVecs(n, vecs, NUM_READ_IOVEC);

    BufferChain **chainp = lastWithData_;

    n = ::readv(sockfd, vecs, nvecs);
    if (n < 0) {
        LOG_ERROR << "readv error";
        return result;
    }

    size_t remain = n;
    //调整chain的off
    for (int i = 0; i < nvecs; ++i) {
        size_t ChainSpace =
            (*chainp)->bufferSize() - (*chainp)->misalign() - (*chainp)->off();
        // if (ChainSpace > BUFFER_MAX_READ) ChainSpace = BUFFER_MAX_READ;
        if (ChainSpace < remain) {
            (*chainp)->off_ += ChainSpace;
            remain -= ChainSpace;
        }
        else {  //最后一个
            (*chainp)->off_ += ChainSpace;
            lastWithData_ = chainp;
            break;
        }

        chainp = &(*chainp)->next_;
    }

    totalSize_ += n;
    result = n;
    return result;
}

}  // namespace net
