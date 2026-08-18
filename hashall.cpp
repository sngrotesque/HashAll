#include <openssl/evp.h>

#include <filesystem>
#include <fstream>
#include <algorithm>  // std::transform
#include <cctype>     // ::tolower
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

// macro definition

#if defined(_WIN64) || defined(_MSC_VER)
#    include <Windows.h>
#    pragma comment(lib, "libcrypto")
#    pragma comment(lib, "libssl")
#    if (_MSVC_LANG < 202002L) && (__cplusplus < 202002L)
#        error "Please enable C++20 or above standard to use this project."
#    endif
#else
#    if (__cplusplus < 202002L)
#        error "Please enable C++20 or above standard to use this project."
#    endif
using BYTE = unsigned char;
#endif

#include <format>

// type definition

namespace fs = std::filesystem;

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
        auto to_hex = [](const std::string &data) -> std::string {
            auto to_char = [](BYTE c) {
                return (c + 0x57) - (-(c < 0xa) & 0x27);
            };

            size_t old_length = data.size();
            std::string result(old_length * 2, '\0');

            for (size_t i = 0; i < old_length; ++i) {
                result[i * 2] = to_char(static_cast<BYTE>(data[i]) >> 4);
                result[i * 2 + 1] = to_char(static_cast<BYTE>(data[i]) & 0xf);
            }

            return result;
        };

        std::string _digest = this->digest();
        return to_hex(_digest);
    }
};

// Using a list of major hash algorithms, remove some algorithms that may be
// incorrect or unsupported.
const std::vector<std::string> algos = {
    "md5",      "sha1",   "sha224",   "sha3-224", "sm3",      "sha256",
    "sha3-256", "sha384", "sha3-384", "sha512",   "sha3-512",
};
// Read in blocks of 16MB
constexpr size_t CHUNK_SIZE = 16777216ULL;

static void hashall_all(const fs::path &path)
{
    std::string path_str = path.string();

    char *buffer = new char[CHUNK_SIZE];

    for (const auto &name : algos) {
        const EVP_MD *md = EVP_get_digestbyname(name.data());
        if (!md) {
            std::cerr << std::format("Skipping unsupported: {}\n", name);
            continue;
        }

        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) {
            delete[] buffer;
            throw std::runtime_error(std::format("Cannot open file: {}\n", path_str));
        }

        Hashlib hasher(md);
        while (file.read(buffer, CHUNK_SIZE) || (file.gcount() > 0)) {
            hasher.update(reinterpret_cast<BYTE *>(buffer), file.gcount());
        }

        std::cout << std::format("{0:<8} [{1}] {2}\n", name, hasher.hexdigest(), path_str);
    }

    delete[] buffer;
}

static void hashall_single(const std::string &algo, const fs::path &path)
{
    std::string path_str = path.string();

    char *buffer = new char[CHUNK_SIZE];

    const EVP_MD *md = EVP_get_digestbyname(algo.data());
    if (!md) {
        delete[] buffer;
        throw std::runtime_error(std::format("Unknown algorithm: {}\n", algo));
    }

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        delete[] buffer;
        throw std::runtime_error(std::format("Cannot open file: {}\n", path_str));
    }

    Hashlib hasher(md);
    while (file.read(buffer, CHUNK_SIZE) || file.gcount() > 0) {
        hasher.update(reinterpret_cast<BYTE *>(buffer), file.gcount());
    }

    std::string &&hexdigest = hasher.hexdigest();
    std::cout << std::format("{0:<8} [{1}] {2}\n", algo, hexdigest, path_str);

    delete[] buffer;
}

// clang-format off
int main(int argc, char **argv)
{
    if (argc < 3) {
        std::cerr << std::format(
            "Usage: {0} <algorithm> <file_path>\n"
            "\tAuthor: SN-Grotesque (github: sngrotesque)\n"
            "\tParam: <algorithm>: The name of the hash algorithm (e.g., md5, sha256), other: all\n"
            "\tParam: <file_path>: The path to the file to be hashed\n"
            "\tExample: {0} sha256 C:\\Windows\\explorer.exe\n"
            "\t         {0} all C:\\Windows\\explorer.exe\n",
            argv[0]);
        return 1;
    }
    std::string algo(argv[1]);
    fs::path path(argv[2]);

    if (!fs::exists(path)) {
        std::cerr << std::format("Path {} not found.\n", path.string());
        return 1;
    }
    std::transform(algo.begin(), algo.end(), algo.begin(), [](BYTE c) { return std::tolower(c); });

    try {
        auto f_size = static_cast<double>(fs::file_size(path));

        std::cout << std::format(
            "{0:<8} {1:.0f} Bytes, {2:.2f} KB, {3:.2f} MB\n",
            "length",
            f_size,
            f_size / 1024,
            f_size / 1024 / 1024
        );

        if (algo == "all") {
            hashall_all(path);
        } else if (std::find(algos.begin(), algos.end(), algo) != algos.end()) {
            hashall_single(algo, path);
        } else {
            std::cerr << std::format("Unsupported parameter: {}.\n", algo);
            return 1;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
