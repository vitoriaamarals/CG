<h1>V-ART: Uso no Windows</h1>
<h2>📜 Pré-requisitos</h2>
<p>Antes de começar, garanta que o seu ambiente está configurado com:</p>

    1. WSL com Ubuntu: Instalado e configurado com um usuário e senha.
    2. Pacotes de Desenvolvimento: Todas as bibliotecas e compiladores necessários instalados no Ubuntu.

        sudo apt update
        sudo apt install build-essential freeglut3-dev libglew-dev libdevil-dev libxerces-c-dev

    3. VcXsrv (X Server): Instalado no Windows para permitir a exibição das janelas gráficas.

<h2>🚀 Compilando um Novo Exemplo (Primeira Vez)</h2>

    1. Abra o terminal Ubuntu (WSL).
    2. Navegue até a pasta do exemplo desejado.

        # Exemplo para a pasta 'keyboard'
        cd /mnt/d/Vitoria/UFLA/CG/desgraca/examples/keyboard

    3. Compile

            make

<h2>🏃‍♂️ Rotina Diária (Executando um Projeto Existente)</h2>

    1. No Windows: Inicie o servidor gráfico.

        Abra o "XLaunch" pelo Menu Iniciar.
        Clique em "Avançar" em todas as telas, deixando as opções padrão (especialmente com "Disable access control" marcado).
        Garanta que o ícone do "X" apareceu perto do relógio do Windows.

    2. No Linux WSL: Abra o terminal Ubuntu (WSL).

    3. Configure o "Display". Diga ao Linux para enviar os gráficos para a tela do Windows.

        export DISPLAY=localhost:0.0
        export LIBGL_ALWAYS_INDIRECT=1

    Obs: Só é necessário fazer isso se der erro, porque já coloquei as instruções no arquivo de config do shell.

    4. Navegue até a pasta do projeto.

        cd /mnt/d/Vitoria/UFLA/CG/desgraca/examples/animation

    5. Execute o programa!

        make

<h2>✨ Dica de Ouro: Automatizando os Comandos export</h2>

    (Já fiz, mas se precisar de novo:)

    Para não ter que digitar os comandos export toda vez que você abre o terminal, você pode adicioná-los ao seu arquivo de configuração do shell.
    Rode este comando uma única vez no seu terminal Ubuntu:

        echo -e '\n# Configura o display para rodar apps gráficos\nexport DISPLAY=localhost:0.0\nexport LIBGL_ALWAYS_INDIRECT=1' >> ~/.bashrc

    Depois disso, feche e abra o terminal Ubuntu novamente. A partir de agora, os comandos export serão executados automaticamente em toda nova sessão!

