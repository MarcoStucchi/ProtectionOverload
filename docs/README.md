# Documentation
:dart: Target of this project is to learn about **Test** and **Test Automation** in embedded world. 
The application is a Circuit Breaker Overload Protection as per standard IEC 60947-2.

## Main features
- **Windows** and **ARM** GCC toolchains used for tests (regardless of production toolchain)
- Test suite: **Unity** 🚀
- ARM emulation with **QEMU**

## Testing process 

### Development process
Basic development process uses specific IDEs and compiler to generate an executable file for the target platform (e.g. Keil MDK and ARM LLVM C compiler).

![image](https://github.com/user-attachments/assets/61693346-283c-4f31-ab6a-b950c5782a5c)

### Test Process
On the other hand, testing process is typically based on a sepataed toolchain, and uses dedicated test files able to test a dedicated set of source files in an isolated environment, that is not necessarily the target environment. Typically the environment is the your desktop PC, or a cloud machine and the toolchain includes GCC.

![image](https://github.com/user-attachments/assets/150ecabb-f542-4af1-9e36-275778fe1ce0)


# Links
WinLibs standalone build of GCC and MinGW-w64 for Windows - https://winlibs.com/

Arm GNU Toolchain Downloads - [https://developer.arm.com/](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)

Unity Assertion Reference - [docs/UnityAssertionsReference.md](https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md)
