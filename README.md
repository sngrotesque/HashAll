# HashAll

A file hash tool used as a replacement for xxx-sum.

# Compile

### Windows

```bat
@REM 填写你自己的实际头文件目录路径
set ssl_incl=""

@REM 填写你自己的实际库目录路径
set ssl_lib=""

cl hashall.cpp /I %ssl_incl% /EHsc /std:c++20 /link /LIBPATH:%ssl_lib% libcrypto.lib libssl.lib
```

### Other (Linux, Mac OS, Android Termux...)

```bash
g++ hashall.cpp --std=c++20 -lssl -lcrypto -o hashall
```

# Example

```bash
C:\Users\sn\Desktop>hashall
Usage: hashall <algorithm/all> <file_path>
        Author: SN-Grotesque (github: sngrotesque)
        Param: <algorithm>: The name of the hash algorithm (e.g., md5, sha256), other: all
        Param: <file_path>: The path to the file to be hashed
        Example: hashall sha256 C:\Windows\explorer.exe
                 hashall all C:\Windows\explorer.exe

C:\Users\sn\Desktop>hashall sha256 pixiv.txt
e8aade4183b04d2a53169fdfcd6f5529a69126ad85aba0713a05008289f2c4e4 pixiv.txt

C:\Users\sn\Desktop>hashall all pixiv.txt
md5        4b028a0bd34263b4bc15c03d5a92e2a1                                                                                                 pixiv.txt
sha1       50e62fdbe596e0d4a1adb8df639cafa8a998e43e                                                                                         pixiv.txt
sha224     4074380e8a97b9079b0a4904aef5cc671b3469391b66eaae3256c644                                                                         pixiv.txt
sha3-224   1735957a7f68e9c7b755b45bf9d0923a0d5f05bc531a8f999ea2822a                                                                         pixiv.txt
sm3        e456b26d78fe1946a8a04e78554bb9f846627d7c1d74edaf2adea336d0e4e1f6                                                                 pixiv.txt
sha256     e8aade4183b04d2a53169fdfcd6f5529a69126ad85aba0713a05008289f2c4e4                                                                 pixiv.txt
sha3-256   4540c424d5d883423e7844032d4abdbd073c66c07b3f0f8193b7fb3dc5964300                                                                 pixiv.txt
sha384     58ef012c98bbf5ba515f22926cf3030b2adc907680608d924eec8858c7a43bea6ce5937f7e4d70560ce4200de316f65e                                 pixiv.txt
sha3-384   6bd2ac66887341a32ae6b1f55f41bd1c6605bf31569c8a6fcaa79c8171fc7124416078c5fbacabe7014b58904b536c3d                                 pixiv.txt
sha512     e847f76ff36c3d6cf40345e3e700698a4bb9bd4013391bb7f4a0f57fdf2ddb1b09601afe79b7fd9d4a7f038827a3e30560306c75fabd3417e7da216fe1240339 pixiv.txt
sha3-512   b88a31269e345c7f6b6539d21e7061bb9867a43c23ae0354c6fece14d3fb396c8190b40ebcb49714e628208116e6a544f3c13428f93fdfb2f021fd1a19a7175b pixiv.txt
```
