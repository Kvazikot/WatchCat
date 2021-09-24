# WatchCat
vsbaranov83@gmail.com Владимир Баранов

Программа watchcat нужна, если вы хотите узнать кто входил в вашу комнату пока вас не было.
Возможно вы хотите знать кто садился за кмпьютер.
Все что необходимо для ее работы это компьютер с linux и веб камера.
Если у вас современный ноутбук с камерой, то веб камера вам не нужна.
При появлении значимого движения в кадре, программы включает сирену
и начинает высылать предупреждения на email.
Звуковой файл сирены можно определить опцией -f
Можно записывать видео на диск или пробрасывать его на stdout.
 
```
Usage: WatchCat.exe [params]

        -d, --show_date
                print date
        --delay, --dt (value:120)
                set delay before alarm
        -e, --email (value:0)
                email me
        --email_script, -p (value:0)
                path to email script
        -f, --sound_file (value:0)
                sound file
        --fps, --frame_rate (value:25)
                25 frames per second
        -h, --help (value:true)
                print help message
        -i, --input (value:0)
                take input video or stream
        --no_sound, --ns (value:0)
                don't play sound
        --no_video, --nv (value:0)
                don't show video
        -o, --output
                save output video
        -r, --resolution (value:640_480)
                resolution 640_480
        -s, --stdout (value:0)
                std output in jmpeg
```



## Проект watchcat
OS: Linux
OS: Windows (experimental mode)
[скачать Watchcat_Release_1.05_win64.zip](https://github.com/Kvazikot/WatchCat/tags)

Проект на с++ использует вебкамеру
Для сборки нужен opencv и cmake.

python нужен чтобы запустить внешний скрипт send_email

## Основные функции 
1. запись видео при срабатывании триггера
   аудио триггер (порог громкости превышает заданное значение)
   видео триггер (в кадре есть движение)
   речевой триггер( есть определенные слова или сочетания слов в речи )
   использовать пайп в ffmpeg

2. Передача емайла при срабатывании тригера (посмотреть скрипт на пайтоне)

3. Включение сирены (сирена сначала будет мой голос запитченный на пару тонов для устрашения)

## Обработка видео
Библиотека opencv 
Дополнительные библиотеки для motion tracking и идентификации по лицу.

## Как настроить почту?
Чтобы работала функция отправки почты необходимо: 
1. Установить интерпретатор python 3.7 с официального сайта.
2. Найти провайдера который предоставляет бесплатный smtp-server. Например mailjet.com.
3. Зарегистируйте на нем свой smtp ящик.
4. Отредактировать файл send_mail.py
В строчке login нужно прописать те значения которые дает сервер при регистрации ящика.
На конкретном сервисе сейчас эта информация доступна по ссылке https://app.mailjet.com/account/setup
В поле Credentials (API and SMTP) скопируйте два длинных идентификатора Username и Password.
Найдите строчку в send_mail.py и вставте СВОИ(!!) идентификаторы.
```
server.login('24ed635ed6e3e35437ce2e33415ea422', "16748719e3322b46a173b5e6791e7f85")
```
В этом же фале вы можете отредактировать текст сообщения
Редактируйте поля FROM, TO, SUBJECT, TEXT.
