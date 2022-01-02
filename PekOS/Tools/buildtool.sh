
sudo apt-get install nasm && continue || echo FAILED to install nasm
sudo apt-get install qemu-system-x86 && continue || echo FAILED to install Qemu
sudo apt-get install git && echo git insatlled successfully || echo FAILED to install git

git clone https://github.com/StjepanBM1/PekOS

cd PekOS
cd PekOS
cd Tools
echo "PekOS cloned successfully"
