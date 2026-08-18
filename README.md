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
C:\Users\sn>hashall -h
Usage: hashall <algorithm> <file_path>
        Author: SN-Grotesque (github: sngrotesque)
        Param: <algorithm>: The name of the hash algorithm (e.g., md5, sha256), other: all
        Param: <file_path>: The path to the file to be hashed
        Example: hashall sha256 C:\Windows\explorer.exe
                 hashall all C:\Windows\explorer.exe

C:\Users\sn>hashall all "F:\Pitchers\bg.jpg"
length   797776 Bytes, 779.08 KB, 0.76 MB
md5      [c355a5c6f3ef333c90d48d02c369dc5a] F:\Pitchers\bg.jpg
sha1     [c65fac72aa1001913500ad3b53c965e7d538d48e] F:\Pitchers\bg.jpg
sha224   [e5a377bfd0bfdcb5683590255af6b62069146285a539321c95c4a67d] F:\Pitchers\bg.jpg
sha3-224 [bd07ed396f8739bd20efc061f2bf053f7406b4a8f229043a5fb6677b] F:\Pitchers\bg.jpg
sm3      [439874a1e925a644c6f505f7cc7106d0de21f7886782bf3be53cbbf9ca6e70a0] F:\Pitchers\bg.jpg
sha256   [a40e9781d067f6ed551e5a175fe70544a9cfbee9985f08f7c97bb383effe8a21] F:\Pitchers\bg.jpg
sha3-256 [c499636601236ed7991bbe1bf47474c231db26560cd06f9705c23330f77bbb36] F:\Pitchers\bg.jpg
sha384   [ee8309b27d579ab8d4f21af90c539ff9c227db2e65d2edcffa546abff6188dfd84850ae53acffb94012b6854ab71d301] F:\Pitchers\bg.jpg
sha3-384 [20851a7ceafa41f60e72ea8c301031ce16de0666ce97011ac8e421f0286c39c31b4f703d2ad45668f1398a7f4c2a27c1] F:\Pitchers\bg.jpg
sha512   [dc93011c422a25c6f63d309dfdfad5af16fc5729db3701aed20b4e237eb75146cde3d0a045ca8fa056d1e23a0eb728fc187e215d8a4e2b40d309237c525f24dc] F:\Pitchers\bg.jpg
sha3-512 [b6586d35e67bd77f06e5064f1fdbeb9747495b58dc20537c52f9b2b2d827f53c93f935ad4b5d35a02a3d1daf1e8b9a8aaf8d9c14fcadf5431e8bb656259e5350] F:\Pitchers\bg.jpg

C:\Users\sn>hashall sha256 "F:\Pitchers\bg.jpg"
length   797776 Bytes, 779.08 KB, 0.76 MB
sha256   [a40e9781d067f6ed551e5a175fe70544a9cfbee9985f08f7c97bb383effe8a21] F:\Pitchers\bg.jpg

C:\Users\sn>hashall sha1 "F:\Pitchers\bg.jpg"
length   797776 Bytes, 779.08 KB, 0.76 MB
sha1     [c65fac72aa1001913500ad3b53c965e7d538d48e] F:\Pitchers\bg.jpg
```
