# Homework 1 - Markdown guide to setup a Linux machine in CloudVeneto for C Code Compilation and Execution

This guide provides step-by-step instructions to prepare a Linux Virtual Machine on CloudVeneto for compiling and running C programs.

## 1. Initiating Your CloudVeneto Machine
- Log in to the CloudVeneto portal ([https://cloudveneto.it](https://cloudveneto.it))
- Select **Project** → **Compute**
  
### 1.1 Create an SSH Key Pair
- Select **Key Pairs** → **Create Key Pair**
- Give the Key Pair a name (e.g. my_key) and select SSH Key as Key Type
- A file `my_key.pem` will be downloaded to your computer and should be moved to a safe location
- Run
  ```bash
  chmod 600 my_key.pem
  ```
  to keep it private.

### 1.2 Create an Instance
- Select **Instances**→ **Launch Instance**
- Manage the settings of your Virtual Machine as follows:
  1. **Details** → **Instance Name:** MyLinuxVM
  2. **Details** → **Counts:** # of VMs you want to start
  3. **Source** → **Select Boot Source:** Image or Instance Snapshot
  4. **Source** → **Available:** AlmaLinux
  6. **Flavor:** → **Available:** select a large flavour
  7. **Key Pair:** select your key
  8. **Networks:** → **Available:** `<ProjectName>-lan`
- Click on **Launch Instance**
  
Congratulations! You have just created your first Virtual Machine on CloudVeneto. You can now see its IP address.

For any doubt check the detailed guide at [https://userguide.cloudveneto.it/en/latest/](https://cloudveneto.it)).

## 2. Access your Virtual Machine via SSH
- Open a terminal tab from your PC
- Copy the file `my_key.pem` in the gate machine (credentials for accessing were sent to you when your account on the cloud was created):
  ```bash
  scp /yourpathto/my_key.pem username@gate.cloudveneto.it:~
  ```
- Use SSH to access the gate machine:
  ```bash
  ssh username@gate.cloudveneto.it
  ```
- - Move the key to a safe directory and make it private:
  ```bash
  mkdir private
  chmod 600 my_key.pem
  mv ~/my_key.pem ~/private
  ```
- Once you are logged in the gate machine, you can access your AlmaLinux VM through:
  ```bash
  ssh -i ~/private/my_key.pem almalinux@VM_IPaddress
  ```
You are finally logged in your Virtual Machine.

## 3. Updating the System
It is best practice to update your system before installing new packages:
```bash
sudo dnf update && sudo dnf upgrade -y
```

## 4. Installing GCC (GNU Compiler Collection)
To compile C programs, you need to install gcc:
```bash
sudo dnf install build-essential -y
```
This package includes `gcc`, `g++`, and essential build tools.

### 4.1 Verifying the Installation
Confirm installation by checking the gcc version:
```bash
gcc --version
```
You should see gcc version info output.

## 5. Writing, Compiling and Running a simple C Code
- Create a new C file, for example:
  ```bash
  sudo dnf install nano
  nano hello.c
  ```
- Add your C code. Example:
  ```c
  #include <stdio.h>
  int main() {
      printf("Hello World!\n");
      return 0;
  }
  ```
- Save and exit (Ctrl+O, Enter, Ctrl+X in nano).

- Compile the C program:
  ```bash
  gcc hello.c -o hello
  ```
  This creates an executable named `hello`.

## 6. Running Your Compiled Program
Run the program with:
```bash
./hello
```
You should see: `Hello World!`
