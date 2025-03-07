namespace cli {
    void help();

    namespace key {
        void create();
        void list();
        void print(const std::string& name, bool publicKey, bool privateKey);
    }
}