# <h1 align="center">Trabalho de lista duplamente encadeada</h1>

> Esse trabalho foi proposto na matéria de Estrutura de Dados pelo
> professor Leonardo Medeiros.

## Como executar?

### Dependências

- CMake
  - Ubuntu
    Como é necessário uma versão superior à 3.12.X o cmake fornecido pelo Ubuntu
    não será suficiente. Para isso é necessário [manter uma versão atualizada](https://anglehit.com/how-to-install-the-latest-version-of-cmake-via-command-line/).
  - Arch
    Baixe o cmake pela AUR. `pacman|yay -S cmake`.
- make
  - Ubuntu
    make já vem na `build-essential` caso já tenha a instalado já terá o make.
    
    `sudo apt|apt-get install build-essential` ou
    
    `sudo apt|apt-get -y install make` ou
    
    `yum install make`.
  - Arch
    `pacman|yay -S make`.

### Buildar o projeto

```sh
~ $ cd Routes
~/Routes $ mkdir build && cd build
~/Routes/build $ cmake ..
~/Routes/build $ make
```

### Executar o projeto

```sh
~/Routes $ ./run
```

Caso o arquivo não esteja com permissão de execução: `~/Routes $ chmod +x run`

Ou

```sh
~/Routes $ make -C build/
~/Routes $ ./build/Routes
```
