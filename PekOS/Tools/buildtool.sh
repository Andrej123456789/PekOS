
sudo apt-get install nasm && continue || echo NASM INSTALLATION FAILED
sudo apt-get install qemu-system-x86 && continue || echo QEMU INSTALLATION FAILED
sudo apt-get install git && echo git insatlled successfully || echo GIT INSTALLATION FAILED

git clone https://github.com/StjepanBM1/PekOS

cd PekOS
cd PekOS
cd Tools
echo "PekOS cloned successfully"
