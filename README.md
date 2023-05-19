# Sobre
Este projeto é a implementação de um servidor web socket. Ele fornece acesso aos arquivos de uma pasta especificada por parâmetro na execução do programa, ou caso não seja especificada uma pasta, fornece acesso a pasta em que está localizado. O acesso é feito via rede, através de protocolo TCP e comunicação bidirecional por canais full-duplex. Por questões de segurança, a implementação não permite que sejam acessadas pastas que estão do lado de fora do caminho especificado, só é possível navegar pelas pastas internas. Para que a aplicação funcione **_é necessário que seja executada em uma máquina com sistema operacional Linux_**.

# About
Implementation of a socket web server. It provides access to files in a folder specified by parameter at program execution, or if a folder is not specified, it provides access to the folder in which it is located. Access is via the network, using the TCP protocol and bidirectional communication through full-duplex channels. For security reasons, the implementation does not allow accessing folders that are outside the specified path, it is only possible to browse the internal folders. To work, the application **_must run on a machine with a Linux operating system_**.

# Como usar?

Para compilar o projeto na [IDE CodeBlocks](https://www.codeblocks.org/downloads/) ou diretamente no terminal, é necessário inserir o parâmetro "-pthread" para que o compilador consiga reconhecer adequadamente a biblioteca "thread" que foi utilizada neste projeto.

Esse parâmetro não vem setado por padrão no CodeBlocks. Por isso, aqui vai um breve tutorial de como fazer essa configuração para compilar o projeto corretamente (se não estiver usando CodeBlocks pode pular essa esse tutorial):

- Abra o projeto e selecione a opção Project->Build options...
- Em seguida será aberta uma janela chamada "Project build options"
- Nessa janela, selecione a aba "Linker Settings"
- Selecione o campo chamado "Other linker options:" e escreva o parâmetro "-pthread" sem aspas
- Clique em Ok para salvar as configurações. Agora o projeto deve compilar corretamente.

Caso queira usar o terminal do Linux para compilar, será necessário usar apenas o arquivo "Trabalho.cpp". Basta executar o comando `g++ -std=c++11 -pthread Trabalho.cpp -o nomeexecutavel`.

**_Obs:_** Conforme especificado, para iniciar o servidor na pasta desejada basta inserir o diretório diretamente nos parâmetros quando for executar o programa (ex: `nomeexecutavel /home/pastadesejada`). Caso não informe um diretório o programa será iniciado na pasta do projeto que contém o arquivo "main.cpp" ou "Trabalho.cpp".

Servidor criado por: Carlos Willian Silva Camargos, Rafael Borges de Assis e Lucas Dael'olio de Lima.
