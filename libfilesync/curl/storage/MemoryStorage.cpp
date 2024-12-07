#include <libfilesync/curl/storage/MemoryStorage.hpp>
#include <libfilesync/curl/Exception.hpp>
#include <libfilesync/utility/Debug.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace filesync::curl::storage {

    MemoryStorage::MemoryStorage(CharBuffer& data) :
        data{data} {

    }

    void MemoryStorage::doSetupRead(const option::Factory& optionFactory) {
        optionFactory.createGeneric(CURLOPT_READFUNCTION, &memoryStorageReadCallback)->set();
        data.resetReadPosition();
        optionFactory.createGeneric(CURLOPT_READDATA, static_cast<void*>(&data))->set();
    }

    void MemoryStorage::doSetupWrite(const option::Factory& optionFactory) {
        optionFactory.createGeneric(CURLOPT_WRITEFUNCTION, &memoryStorageWriteCallback)->set();
        data.resetReadPosition();
        DEBUG("Data address: " << &data);
        optionFactory.createGeneric(CURLOPT_WRITEDATA, static_cast<void*>(&data))->set();
    }

    void MemoryStorage::doFlush() {
        /* Do nothing */
    }

    /**
     * @brief Write callback function which writes incoming
     * data to a memory chunk.
     * 
     * This implementation is based on the `getinmemory.c` 
     * example from the libcurl documentation examples section.
     * 
     * @param[in] target Target memory which the recieved data
     * is written to. If it is NULL, we discard the received
     * data.
     */
    extern "C" size_t memoryStorageWriteCallback(char* contents,
        size_t size, size_t count, void* target) {

        DEBUG_ENTER();
        size_t ret = 0;

        if (target) {
            std::size_t dataSize = size * count;
            struct CharBuffer* data = static_cast<CharBuffer*>(target);
            
            ret = data->write(contents, dataSize);
            
        } else {
            ret = count;
        }

        DEBUG("Return: " << ret);
        DEBUG_EXIT();
        return ret;
    }

    /**
     * @brief Read callback function which reads outgoing
     * data from memory.
     * 
     * @param[in] contents Memory which the outgoing data
     * is read from.
     */
    extern "C" size_t memoryStorageReadCallback(char* buffer,
        size_t size, size_t count, void* contents) {

        DEBUG_ENTER();
        size_t ret = 0;

        if (contents && buffer) {
            struct CharBuffer* data = static_cast<CharBuffer*>(contents);

            std::size_t bufferSize = size * count;

            ret = data->read(buffer, bufferSize);
        } else {
            ret = 0;
        }

        DEBUG("Return: " << ret);
        DEBUG_EXIT();
        return ret;
    }

}