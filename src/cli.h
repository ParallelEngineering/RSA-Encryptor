
class cli {
public:
    static void help();

    class key {
    public:
        static void create();
        static void list();
        static void print(std::string name, bool publicKey, bool privateKey);
    };
};
