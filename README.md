 Игра "Лабиринт", Местай Мурзабаев, 318 группа
 
![](readme_files/Aspose.Words.ebb8c6f6-ed29-4f90-9b5f-ec4260877ada.001.png)

Для сборки и запуска введите shell команды:

- cmake CmakeLists.txt
- make
- ./bin/main

Реализовн 2d-лабиринт из комнат с ловушками и общей схемой схемой схемой

I H O P Q

F G N R S

E J M U T

D K L V W

C B A Y X,  где A  — стартовая и финишная комната, в которую надо вернуться пройдя путь A-B-C-D-E-F-G-J-K-L-M-N-R-S-T-U-V-W-X-Y-A.

В игре реализованы все основные пункты задания, в частности активируемые ловушки.

Для поражения достаточно встать на ловушку, для победы необходимо пройти весь путь и коснуться флажка в комнате А (см. рис. выше).

![](readme_files/Aspose.Words.ebb8c6f6-ed29-4f90-9b5f-ec4260877ada.002.png)![](readme_files/Aspose.Words.ebb8c6f6-ed29-4f90-9b5f-ec4260877ada.003.png)

Ловушки (тайлы "t") активируются при прохождении опреденных активаторов (тайлы "а").

Также реализована спрайтовая анимация ходьбы персонажа.

![](readme_files/Aspose.Words.ebb8c6f6-ed29-4f90-9b5f-ec4260877ada.004.png)![](readme_files/Aspose.Words.ebb8c6f6-ed29-4f90-9b5f-ec4260877ada.005.png)![](readme_files/Aspose.Words.ebb8c6f6-ed29-4f90-9b5f-ec4260877ada.006.png)![](readme_files/Aspose.Words.ebb8c6f6-ed29-4f90-9b5f-ec4260877ada.007.png)

Конец прохождения:

![](readme_files/Aspose.Words.ebb8c6f6-ed29-4f90-9b5f-ec4260877ada.008.png)![](readme_files/Aspose.Words.ebb8c6f6-ed29-4f90-9b5f-ec4260877ada.009.jpeg)
