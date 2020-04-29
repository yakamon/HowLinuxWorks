# prompt
export PROMPT="%n@%M %F{4}%~%F{sgr0} $ "

# load profile
export ZPROFILE="${HOME}/.zprofile"
export ZSHRC="${HOME}/.zshrc"
[ -f ${ZSHRC} ] && . ${ZSHRC}

# locale
export LANG=en_GB.UTF-8

# python
export PYENV_ROOT="${HOME}/.pyenv"
export PATH="${PATH}:${PYENV_ROOT}/bin"
if command -v pyenv 1>/dev/null 2>&1; then
	eval "$(pyenv init -)"
fi

# golang
export PATH="${PATH}:/usr/local/go/bin"
export GOPATH="$(go env GOPATH)"
export PATH="${PATH}:${GOPATH}/bin"
