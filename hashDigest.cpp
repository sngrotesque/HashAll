#include <openssl/evp.h>
#include <cstdint>

#include <filesystem>
#include <fstream>
#include <algorithm>  // std::transform
#include <cctype>     // ::tolower
#include <string>
#include <vector>

#include <iostream>

using u8 = uint8_t;

#if defined(_WIN64) || defined(_MSC_VER)
#    include <Windows.h>
#    pragma comment(lib, "libcrypto")
#    pragma comment(lib, "libssl")
#    define HASHALL_CPP _MSVC_LANG
#else
#    define HASHALL_CPP __cplusplus
#endif

#if HASHALL_CPP >= 202002L
#    include <format>
#endif

class Hashlib {
private:
    const EVP_MD *md = nullptr;
    EVP_MD_CTX *ctx = nullptr;

public:
    Hashlib(const EVP_MD *algo)
    {
        this->ctx = EVP_MD_CTX_new();
        this->md = algo;

        EVP_DigestInit_ex(this->ctx, this->md, nullptr);
    }

    ~Hashlib()
    {
        EVP_MD_CTX_free(this->ctx);
    }

public:
    void update(const BYTE *buffer, size_t length) noexcept
    {
        EVP_DigestUpdate(this->ctx, buffer, length);
    }

    std::string digest() const noexcept
    {
        int digest_size = EVP_MD_size(this->md);
        std::string result(digest_size, '\0');

        EVP_DigestFinal_ex(this->ctx, reinterpret_cast<BYTE *>(result.data()), nullptr);

        return result;
    }

    std::string hexdigest() const noexcept
    {
        std::string _digest = this->digest();
        return to_hex(_digest);
    }
};

static inline BYTE to_char(BYTE c)
{
    return (c + 0x57) - (-(c < 0xa) & 0x27);
}

static std::string to_hex(const std::string &data)
{
    size_t old_length = data.size();
    std::string result(old_length * 2, '\0');

    for (size_t i = 0; i < old_length; ++i) {
        result[i * 2] = to_char(static_cast<BYTE>(data[i]) >> 4);
        result[i * 2 + 1] = to_char(static_cast<BYTE>(data[i]) & 0xf);
    }

    return result;
}

static std::string WcharToChar(std::wstring wstr)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(len, '\0');

    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, str.data(), len, nullptr, nullptr);
    return str;
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        std::cerr << std::format(
            "Usage: {0} <algorithm/all> <file_path>\n"
            "\tAuthor: SN-Grotesque (github: sngrotesque)\n"
            "\tParam: <algorithm>: The name of the hash algorithm (e.g., md5, sha256), other: all\n"
            "\tParam: <file_path>: The path to the file to be hashed\n"
            "\tExample: {0} sha256 C:\\Windows\\explorer.exe\n"
            "\t         {0} all C:\\Windows\\explorer.exe\n",
            argv[0]
        );
        return 1;
    }

    std::string algo = argv[1];
    std::transform(algo.begin(), algo.end(), algo.begin(), [](BYTE c) {
        return std::tolower(c);
    });
    std::filesystem::path path(argv[2]);
    std::string filenameStr =
        WcharToChar(path.wstring());  // 统一转换为 UTF-8 字符串，避免输出乱码

    // 处理 "all" 选项
    if (algo == "all") {
        // 使用主要的哈希算法列表，去掉一些可能出错或不支持的算法。
        const std::vector<std::string> allAlgos = {
            "md5",
            "sha1",
            "sha224",
            "sha3-224",
            // "sha512-224", // 有点阴间，去掉。
            "sm3",
            "sha256",
            "sha3-256",
            // "sha512-256", // 有点阴间，去掉。
            "sha384",
            "sha3-384",
            "sha512",
            "sha3-512",
        };

        for (const auto &name : allAlgos) {
            const EVP_MD *md = EVP_get_digestbyname(name.c_str());
            if (!md) {
                continue;  // 跳过当前 OpenSSL 不支持的算法
            }

            std::ifstream file(path, std::ios::binary);
            if (!file.is_open()) {
                std::cerr << std::format("Cannot open file: {}\n", filenameStr);
                return 1;
            }

            Hashlib hasher(md);
            char buffer[8192];
            while (file.read(buffer, sizeof(buffer)) || (file.gcount() > 0)) {
                hasher.update(reinterpret_cast<BYTE *>(buffer), file.gcount());
            }

            std::cout << std::format(
                "{0:<10} {1:<128} {2}\n", name, hasher.hexdigest(), filenameStr
            );
        }
        return 0;
    }

    // 单个算法模式
    const EVP_MD *md = EVP_get_digestbyname(algo.c_str());
    if (!md) {
        std::cerr << std::format("Unknown algorithm: {}\n", algo);
        return 1;
    }

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << std::format("Cannot open file: {}\n", filenameStr);
        return 1;
    }

    Hashlib hasher(md);
    char buffer[8192];
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        hasher.update(reinterpret_cast<BYTE *>(buffer), file.gcount());
    }

    std::cout << std::format("{} {}\n", hasher.hexdigest(), filenameStr);
    return 0;
}
