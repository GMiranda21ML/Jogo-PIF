# ⚔️ **Metroid Souls** ⚔️

**Metroid Souls** é um jogo inspirado em *Castlevania: Symphony of the Night*, *Blasphemous* e na franquia *Souls*.

Você assume o papel de um cavaleiro com a missão de salvar uma princesa que foi capturada por monstros e agora está presa em seu castelo.  
Embarque nessa incrível jornada para derrotar criaturas sombrias e enfrentar desafios mortais.

**Você consegue salvar a princesa e restaurar a paz no reino?**


![Jogo](assets\backgroundCutScene\cutSceneBackground.png)

O jogo foi desenvolvido em **C** utilizando a biblioteca gráfica **Raylib**

 ## 🛠️ Compilando

💻 **Instalação em Linux**

1. Certifique-se de ter o GCC, make e git:
   ```
   sudo apt install build-essential git
   ```

2. Instale todas as bibliotecas necessárias:
   ```
   sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
   ```

3. Baixe o código-fonte do Raylib diretamente do GitHub e entre na pasta:
   ```
   git clone --depth 1 https://github.com/raysan5/raylib.git raylib
   cd raylib/src/
   ```

4. Compile a Raylib:
   ```
   make PLATFORM=PLATFORM_DESKTOP
   ```

5. Instale a Raylib:
   ```
   sudo make install
   cd ~
   ```

6. Clone o repositório do jogo:
   ```
   git clone https://github.com/GMiranda21ML/Jogo-PIF.git
   cd Jogo-PIF
   ```

7. Compile e rode o jogo:
   ```
   make run
   ```



Caso ocorra algum problema, consulte a documentação da Raylib para Linux: [Raylib para Linux](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux).


---

## 📜 Menu Principal

A tela de menu oferece as seguintes opções:

![Menu](assets\img\menujogo.png)

- 🟡 **Jogar**: Inicia o jogo.
- ⌨️ **Teclas**: Mostra os comandos e controles.
- 🏆 **Score**: Visualiza os melhores resultados e pontuações.
- ❌ **Sair**: Fecha o jogo.

---

## 🕹️ Como Jogar

### Teclas do jogo:

- Use as teclas **W, A, S, D** para controlar o carro:
  - **W** — Pular
  - **A** — Andar para a esquerda
  - **D** — Andar para a direita
  - **L** — Atacar
  - **E** — Interagir com objetos
  - **R** — Reiniciar (voltar ao menu)
  - **ENTER** — Confirmar 


## 🎬 Vídeo demostrativo

[![Assista ao vídeo no YouTube](https://img.youtube.com/vi/CFFP_qnYuyY/maxresdefault.jpg)](https://youtu.be/CFFP_qnYuyY)

## 🗺️ Estrutura dos Mapas e Progressão

**Metroid Souls** é dividido em **4 mapas interconectados**, desafiando o jogador com inimigos, obstáculos e parkour de precisão.

---

### 🌲 Mapa 1 – Floresta Sombria

- O jogador começa sua jornada na **Floresta**, um ambiente hostil repleto de criaturas corrompidas.
- É uma área introdutória onde o jogador aprende a **lutar**, **pular** e **explorar**.
- Ao final da floresta, o jogador encontra a entrada de um castelo antigo e sombrio.

---

### 🏰 Mapa 2 – Castelo (1º e 2º Andar)

- Dentro do castelo, o jogador percorre os **dois primeiros andares**.
- No primeiro andar, ele encontra uma **parede alta intransponível**.
- Sem conseguir subir, ele deve explorar um **buraco  no chão** no primeiro andar.
- Ao saltar pelo buraco, o jogador cai na próxima área...

---

### ⛏️ Mapa 3 – Caverna

- A caverna é escura e traiçoeira, exigindo **precisão em saltos de plataforma (parkour)**.
- Se o jogador **errar o salto**, ele **cai em espinhos e morre**.
- O jogador deve superar inimigos e plataformas móveis para chegar **ao outro lado da parede do castelo**.

---

### 🏯 Mapa 4 – Terceiro Andar do Castelo

- Após atravessar a caverna, o jogador retorna ao castelo, agora no primeiro andar, mas com acesso ao **segundo andar** por meio de um parkour.
- Ele deve derrotar uma sequência de **inimigos ** e resistir aos últimos desafios do jogo.
- Ao final, encontra a **porta final**, que só pode ser aberta com a **chave adquirida anteriormente**.

---

### 🗝️ Desafio  – Encontrar a Chave

- A chave está escondida em algum lugar entre a floresta, castelo e caverna.
- Sem ela, o jogador **não poderá abrir a porta final e salvar a princesa**.
- Exploração cuidadosa e atenção aos detalhes são recompensadas.

---

**Explore, lute, salte e sobreviva. Você está preparado para enfrentar os horrores de Metroid Souls e salvar a princesa?**

## 📊 Slides de apresentação

[Veja os slides no Canva.](https://www.canva.com/design/DAGoTto-qis/6Rn61_Hm-M_6Z0kySZ2rDg/view?utm_content=DAGoTto-qis&utm_campaign=designshare&utm_medium=link2&utm_source=sharebutton)


## 👥 Equipe de Desenvolvimento

- [**André Avelino**](https://github.com/avelinoandre) 
- [**Caio Mathews**](https://github.com/CaioMathews)
- [**Gabriel Miranda**](https://github.com/GMiranda21ML)