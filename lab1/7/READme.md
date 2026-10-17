# 7. Дано бинарное дерево, элементы которого содержат целые
числа. Найдите сумму элементов этого дерева.

## Простенькая реализация бинарного дерева без балансировки

<img width="362" height="485" alt="image" src="https://github.com/user-attachments/assets/89074acd-73ca-4b64-98eb-f78f4cc6fc57" />

## Заполнение дерева случайными значениями, сразу с подсчетом того, что должны получить на выходе

<img width="423" height="159" alt="image" src="https://github.com/user-attachments/assets/93378c50-7679-4ed5-9d8f-7ce52f5b9613" />

## Идея решения
- разрезать дерево на 4*[кол-во потоков] поддеревьев, которые мы уже посчитаем через потоки
- пока обходим дерево и набираем нужные нам поддеревья сумму вершин сразу считаем в итоговую сумму
- когда получили поддеревья -> обходим их в ширину и считаем локальные суммы
- прибавляем локальные суммы к тому, что получили в процессе нарезки и получаем искомую сумму


## Глобальные переменные

<img width="688" height="207" alt="image" src="https://github.com/user-attachments/assets/902a35db-2f00-4dd1-bf65-3808d3bcc9c2" />

- максимальное кол-во задач берем в 4 раза больше, чтобы каждый поток просчитал по 4 поддерева, с целью уменьшить простои

## Нарезка дерева

<img width="819" height="588" alt="image" src="https://github.com/user-attachments/assets/66a7fb57-cc1d-4244-ba83-26d0d5874732" />

## Потоковая функция

<img width="530" height="548" alt="image" src="https://github.com/user-attachments/assets/cb50c290-2625-4116-8fd0-99f78a1d791f" />

## _tmain

- начальная инициализация

<img width="334" height="124" alt="image" src="https://github.com/user-attachments/assets/36cae178-f548-44a7-bfb1-a6bfc8dc5c4d" />

- разбиваем на поддеревья, если все посчитали в момент нарезки, то выводим результат и выходим

<img width="802" height="219" alt="image" src="https://github.com/user-attachments/assets/21fe7904-dcf9-44d8-b54e-1d65a532f7ee" />


- работа с потоками и вывод результата

<img width="759" height="509" alt="image" src="https://github.com/user-attachments/assets/df4e8543-16c3-4ea9-9a5d-0cc3e8454fc2" />


