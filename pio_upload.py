import os

build_command = 'platformio run'
upload_fs_command = 'platformio run --target uploadfs'
upload_fw_command = 'platformio run --target upload'

os.system(build_command)
os.system(upload_fs_command)
os.system(upload_fw_command)

print('Use AP Mode to configure wifi: 192.168.4.1')