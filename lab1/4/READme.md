# 4. Дана квадратная матрица размерности 4×4. Найдите
определитель этой матрицы


## Идея
- будем считать определитель в 4 потока
- по одному минору 3х3 для каждого
- в конце посчитаем итоговый определитель

## Функция посчета определителя

<img width="774" height="405" alt="image" src="https://github.com/user-attachments/assets/5179553b-6e47-40b9-90ca-637f07be68bc" />

- по полученному номеру столбца выбираем определитель 3х3
- по аналогии разбиваем его на миноры 2х2
- получаем 3 слагаемых в det3_mas
- записываем получившийся минор в det4_mas[№столбца]

## Работа с потоками и вывод результата

<img width="834" height="278" alt="image" src="https://github.com/user-attachments/assets/1710c97f-a362-437f-bea1-3a2ceb5f2f71" />


## Результат работы программы

<img width="279" height="139" alt="image" src="https://github.com/user-attachments/assets/8525af5f-04a2-425a-b74b-484a0e05d039" />
<img width="468" height="170" alt="image" src="https://github.com/user-attachments/assets/c83368dd-1d6c-4ecb-accf-26e5e9a31afd" />

