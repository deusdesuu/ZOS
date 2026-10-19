# 1. Дан текст длиной n символов. Необходимо найти контрольную сумму по данному тексту в виде суммы кодов символов по модулю 256. Для нахождения суммы запустите k потоков, где k < n. Каждый i-й поток, i = 0, 1,…,k-1 должен обрабатывать только символы с номерами i + k *s, где s = 0, 1, 2,... – шаг работы потока

## Инициализация глобальных переменных

<img width="543" height="117" alt="image" src="https://github.com/user-attachments/assets/ce458aff-9dd2-4634-b7b2-8ed675baf05a" />

## Инициализация переменных для работы потоков, критической секции

<img width="328" height="177" alt="image" src="https://github.com/user-attachments/assets/16623ab2-a0e1-4e9d-8882-ff93cac695f0" />

## Функция подсчета суммы символов

<img width="456" height="181" alt="image" src="https://github.com/user-attachments/assets/582207a1-1aba-45c2-99a6-b7fb10bae72b" />

## Работа с потоками и вывод результата

<img width="756" height="317" alt="image" src="https://github.com/user-attachments/assets/a0437d6c-404c-4b9f-ac13-68a79a46e9f7" />

## Результат работы программы

<img width="666" height="57" alt="image" src="https://github.com/user-attachments/assets/56ef293b-5c52-455d-9b9a-74a350f2d9da" />
