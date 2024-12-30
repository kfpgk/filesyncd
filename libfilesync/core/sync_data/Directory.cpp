#include <libfilesync/core/sync_data/Directory.hpp>

namespace filesync::core::sync_data {

    Directory::Directory(const std::filesystem::path& path) :
        DirectoryBase{path},
        Entry{path},
        EntryBase{path} {

    }

    void Directory::doPrint() const {
        Entry::doPrint();
        for(auto it = components.begin(); it != components.end(); it++) {
            it->get()->print();        
        }        
    }

    void Directory::doSetRemoteEntry(const std::filesystem::path& path) {
        Entry::doSetRemoteEntry(path);
        for(auto it = components.begin(); it != components.end(); it++) {
            namespace fs = std::filesystem;
            fs::path childPath = it->get()->getPath();
            fs::path childRemote = path.string() + fs::path::preferred_separator
                + childPath.filename().string();
            childRemote.make_preferred();
            it->get()->setRemoteEntry(childRemote.string());
        }
    }

    void Directory::doSetSyncInProgress() {
        Entry::doSetSyncInProgress();
        for(auto it = components.begin(); it != components.end(); it++) {
            it->get()->setSyncInProgress();
        }
    }

    void Directory::doResetSyncInProgress() {
        Entry::doResetSyncInProgress();
        for(auto it = components.begin(); it != components.end(); it++) {
            it->get()->resetSyncInProgress();
        }
    }

    bool Directory::doGetSyncInProgress() const {
        bool syncInProgress = Entry::doGetSyncInProgress();
        for(auto it = components.begin(); it != components.end(); it++) {
            if (it->get()->getSyncInProgress()) {
                syncInProgress = true;
            }              
        }
        return syncInProgress;
    }

    bool Directory::doLocalDifferentThanPrev() {
        bool changed = Entry::doLocalDifferentThanPrev();
        for(auto it = components.begin(); it != components.end(); it++) {
            if (it->get()->localDifferentThanPrev()) {
                changed = true;
            }              
        }
        return changed;
    }

    bool Directory::doRemoteDifferentThanPrev() {
        bool changed = Entry::doRemoteDifferentThanPrev();
        for(auto it = components.begin(); it != components.end(); it++) {
            if (it->get()->remoteDifferentThanPrev()) {
                changed = true;
            }              
        }
        return changed;
    }

    void Directory::doSetPrevious() {
        Entry::doSetPrevious();
        for(auto it = components.begin(); it != components.end(); it++) {
            it->get()->setPrevious();
        }        
    }

    void Directory::doWriteRemoteBufferToLocal() {
        Entry::doWriteRemoteBufferToLocal();
        for(auto it = components.begin(); it != components.end(); it++) {
            it->get()->writeRemoteBufferToLocal();
        }          
    }

}