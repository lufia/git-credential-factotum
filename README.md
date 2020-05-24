# git-credential-factotum

Git-credential-factotum is a helper tool for Git using the Factotum.

## Installation

For plan9port:

```console
% mk -f mkfile.unix
% mk -f mkfile.unix install
```

For native Plan 9:

```console
% mk
% mk install
```

Then you can set Git's credential helper.

```console
% git config --global credential.helper factotum
```
