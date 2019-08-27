# app-framework
Lightweight Linux app framework based on posix and C/C++ standard libraries


__Eclipse integration instructions__
1. Create a new C/C++ project (C++ Managed build, Empty Project)
2. Configure build settings: Project -> Properties -> C/C++ Build -> Settings
3. Add the desired libs/apps include paths, in the correct/dpendency order: GCC C++ Compiler -> Includes -> Include paths (-l) -> Add.. (+ green button)
4. Configure C++11: GCC C++ Compiler -> Dialect -> Language standard -> ISO C++ (-std=c++0x)
5. Add desired libraries. GCC C++ Linker -> General -> Support for pthread (-pthread)
It’s recomended to use static compilation: GCC C++ Linker -> Miscellaneous -> Linker flags   -static
6. Add or link sourse code folder of desired libs/apps: Project -> Properties -> C/C++ General -> Path and Symbols -> Source Location -> [Add Folder..]/[Link Folder..]
7. Build project

Regards,
-Yarib

