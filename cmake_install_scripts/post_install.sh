#!/bin/bash
set -e

VAR_LOG_DIR=${1}
HOME_DIR=${2}
RC_LOCAL_PATH=/etc/rc.local

rad_path=/"${VAR_LOG_DIR}"/radiation-benchmarks
echo "-- Creating ${rad_path} dir"
set -x
sudo mkdir -p "${rad_path}"/log

chmod ugo+w "${rad_path}"
chmod ugo+w "${rad_path}"/log

# Enable the telnet run reboot
if grep -Fq "/sbin/halt, /sbin/reboot, /sbin/poweroff" /etc/sudoers
then
    # code if found
    echo "No need to enable reboot for this user, already enabled"
else
    # code if not found
    echo "%admin ALL=NOPASSWD: /sbin/halt, /sbin/reboot, /sbin/poweroff" >> /etc/sudoers
fi

# Creating /etc/rc.local
cat > "${RC_LOCAL_PATH}" <<EOF
#!/bin/bash

sudo ${HOME_DIR}/atBoot.sh &

exit 0
EOF

chmod 777 "${RC_LOCAL_PATH}"
chmod 777 "${HOME_DIR}"/atBoot.sh


