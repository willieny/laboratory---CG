# Projeto de Computação Gráfica

## Equipe

- Franklin Nascimento: [Link do Github](https://github.com/WaddFranklin)
- Willieny Barbosa: [Link do Github](https://github.com/willieny)

## Como executar o projeto

Para executar este projeto, certifique-se de que tem as bibliotecas OpenGL e GLUT instaladas na sua máquina. Para isso, basta executar os seguintes comandos:

```shell
sudo apt-get update
sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev libglew-dev libjpeg-dev
```

Feito isto, compile o projeto com o comando:

```shell
g++ main.cpp bibutil.cpp myutil.cpp bibutilNoTex.cpp -o main -lGL -lGLU -lglut -ljpeg
```

Em seguida, basta rodar o comando `./main` para executar o programa.

# Requisitos da AB1

- [x] Navegação usando o teclado/mouse permitindo ao usuário visualizar a parte externa e interna do Laboratório;
- [x] 5 objetos diferentes do laboratório;
- [x] Janelas: a janela deverá ser aberta e fechada usando o teclado;
- [x] 1 Porta: A porta deverá ser aberta ou fechada (baixa rotação) através da ação de um botão do teclado.
- [x] 2 ventiladores de teto rotacionando em baixa velocidade;

# Requisitos da AB2

- [x] Criar a iluminação do Laboratório;
- [x] 3 Fontes de Luz simulando uma luminária com spot sob as Mesas do Laboratório;
- [x] Utilizar Iluminação (especular e difusa);
- [x] Aplicar mapeamento de textura em 5 objetos;
- [x] Criar um modelo de iluminação para simular o efeito noite e dia na modelagem.
- [x] Refinar a modelagem realizada no primeiro Bimestre;
