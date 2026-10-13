# 1. Измените приведенную в примере программу так, чтобы создание строк матрицы происходило параллельно

По аналогии с уже приведенным кодом делаем заполнение матрицы

## 2 массива для хранения id строки и сидов для рандома

<img width="278" height="39" alt="image" src="https://github.com/user-attachments/assets/868e77d0-68fd-495f-a936-354079e414c8" />


## функция для заполнения:
- берем номер строки, которую будем заполнять
- берем соответствующий для этой строки сид для рандома
- заполняем

<img width="326" height="173" alt="image" src="https://github.com/user-attachments/assets/05927e96-d943-4019-9ba9-1eb29adb7944" />


## в main:
- инициализируем массивы для заполнения
- открываем потоки для заполнения
- ждем их окончания
- закрываем
- printmtx() - вывод матрицы для дебага

<img width="766" height="555" alt="image" src="https://github.com/user-attachments/assets/a5beaa19-1725-4cc0-b06e-9592ce68cb40" />


## результат работы:

<img width="1326" height="231" alt="image" src="https://github.com/user-attachments/assets/b02e6316-436c-4cb3-8bc5-7666877ea65a" />
