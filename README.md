# Projeto de Computação Gráfica - AB1

## Equipe

- Franklin Nascimento: [Link do Github](https://github.com/WaddFranklin)
- Willieny Barbosa: [Link do Github](https://github.com/willieny)

## Como executar o projeto

Para executar este projeto, certifique-se de que tem as bibliotecas OpenGL e GLUT instaladas na sua máquina. Para isso, basta executar os seguintes comandos:

```shell
sudo apt-get update
sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev libglew-dev
```

Feito isto, compile o projeto com o comando:

```shell
g++ main.cpp bibutil.cpp myutil.cpp -o main -lGL -lGLU -lglut
```

Em seguida, basta rodar o comando `./main` para executar o programa.