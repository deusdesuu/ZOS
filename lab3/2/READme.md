# 2. Дан файл, содержащий текст произвольной длины и ключ длиной k символов. Выполнить шифрование этого текста по алгоритму, рассмотренному в задаче № 5 лабораторной работы № 1. 
Для решения задачи реализовать три потока:
- поток чтения информации из файла в буфер;
- поток шифрования текста;
- поток вывода шифрованного текста в результирующий файл. 
# 3. Напишите программу, дешифрования текста, полученного в результате выполнения задачи № 2, используя 3 потока.

## Константы

<img width="358" height="246" alt="image" src="https://github.com/user-attachments/assets/a8a07b61-14b8-4540-85ea-042a8e754adb" />

## reader1

<img width="534" height="574" alt="image" src="https://github.com/user-attachments/assets/263d808a-a672-4321-bf6d-66f1571ce1a1" />

- аналогично заданию 1 - ждем buffer_free == true, считываем данные в буфер

## encoder

<img width="486" height="606" alt="image" src="https://github.com/user-attachments/assets/5dd34a46-c4ba-4f39-ad64-89fa8f890a23" />

- ждем, когда данные для обработки будут готовы
- кодируем

## writer1

<img width="518" height="485" alt="image" src="https://github.com/user-attachments/assets/a8d08b11-8849-4539-af10-012a79ec6652" />

- ждем, когда данные в буфере будут закодированы
- записываем в файл

## reader2, decoder, writer2 по аналогии

## Инициализация потоков и вывод результата

<img width="458" height="625" alt="image" src="https://github.com/user-attachments/assets/4b0462cb-52ac-41d3-9d37-21517f1f561f" />

# Результат работы

<img width="1612" height="44" alt="image" src="https://github.com/user-attachments/assets/1eef64ab-cf1a-40b8-a411-8f288fb8a2c8" />

<img width="544" height="216" alt="image" src="https://github.com/user-attachments/assets/5916a5e6-f558-4775-812b-094f53312828" />

<img width="1611" height="49" alt="image" src="https://github.com/user-attachments/assets/b61bc485-7ca9-49c7-90cb-3fc845b378c6" />
