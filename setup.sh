# pyenv
PYENV_SRC="https://github.com/pyenv/pyenv.git"
PYENV_INSTALL_PATH="${HOME}/.pyenv"
git clone ${PYENV_SRC} ${PYENV_INSTALL_PATH}

# go
GO_SRC="go1.14.2.linux-amd64.tar.gz"
GO_INSTALL_PATH="/usr/local"
wget https://dl.google.com/go/${GO_SRC}
tar -C ${GO_INSTALL_PATH} -xzf ${GO_SRC}
rm ${GO_SRC}
