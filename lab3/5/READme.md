# 4. Имеется n потоков, генерирующих случайные целые числа. Занести эти числа в хеш-таблицу, представляющую собой массив из 10 указателей на списки целых чисел. Для определения местоположения числа x в хеш-таблице использовать следующую хеш-функцию.
# Если поток собирается записать число x в k-ю строку таблицы, то он должен проверить, не пишет ли другой поток свое число в эту же строку.
# 5. Модифицировать задачу 4 таким образом, чтобы количество строк в таблицы и основание хеш-функции определялось пользователем. Доступ к строкам хеш-таблицы организовать на основании модели монитора.
# Опишите методы захвата строки OcupateRow и освобождения строки FreeRow. Используйте необходимое количество мьютексов.

# Структура односвязного списка для хеш таблицы

<img width="445" height="684" alt="image" src="https://github.com/user-attachments/assets/f977f27f-5c95-4932-8685-09bf525711db" />

# Константы

<img width="348" height="299" alt="image" src="https://github.com/user-attachments/assets/4db6f692-439f-4271-a3f8-b6f5cf29c903" />

# Добавление элемента в хеш таблицу

<img width="563" height="248" alt="image" src="https://github.com/user-attachments/assets/009c7ec2-c625-4178-9215-809b29615be2" />

# main

<img width="822" height="811" alt="image" src="https://github.com/user-attachments/assets/f63ec5e7-1999-49cf-a786-87fd311d2bdc" />

# CMonitor

## Структура класса

<img width="555" height="397" alt="image" src="https://github.com/user-attachments/assets/baa308b8-a9ef-4806-8118-e41bdd9e79f6" />

* пустые операторы копирования т.к. работаем с ядром

## Конструктор, деструктор, OcupateRow, Freerow

<img width="472" height="495" alt="image" src="https://github.com/user-attachments/assets/a8b16ede-5814-4b16-909a-0df723ae5f56" />

# Результат работы

<img width="648" height="189" alt="image" src="https://github.com/user-attachments/assets/1e502815-e25d-4917-b280-1d50429c5c98" />
