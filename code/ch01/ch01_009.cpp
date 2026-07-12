class FileHandle {
    FILE* f_;
public:
    FileHandle(const char* name, const char* mode)
        : f_(fopen(name, mode)) {
        if (!f_) throw std::runtime_error("Cannot open file");
    }
    ~FileHandle() { if (f_) fclose(f_); }
    
    // No copy (for simplicity)
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
};
