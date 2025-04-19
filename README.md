# Инструкция по запуску консольного приложения (Android ADB)

Это приложение распознает лица на изображении с помощью OpenCV.

## Требования

*   **ADB:** Утилита должна быть установлена и доступна из командной строки.
*   **Android Устройство или Эмулятор:** С включенной отладкой по USB.
*   **Совпадение архитектур:** Собранное приложение (`face_recognition_app`) и устройство/эмулятор должны иметь одинаковую архитектуру (например, оба `x86_64`, на `arm64-v8a` у меня не работало ни в какую).
*   **Файлы в архиве:**
    *   `build-android/face_recognition_app`: Исполняемый файл.
    *   `build-android/haarcascade_frontalface_default.xml`: Каскад Хаара.
    *   Необходимые библиотеки `.so` (должны быть скопированы отдельно).

## Запуск

1.  **Подключите** устройство/эмулятор и убедитесь, что `adb devices` его видит.
2.  **Найдите и подготовьте библиотеки** для нужной архитектуры:
    *   `libopencv_java4.so` (из OpenCV Android SDK)
    *   `libc++_shared.so` (из Android NDK) (я не настолько сильно уверен, что он нужен, но `libopencv_java4.so` ссылается на него)
3.  **Скопируйте все** во временную папку на устройстве или эмуляторе:
    ```powershell

    adb push face_recognition_app /data/local/tmp/
    adb push haarcascade_frontalface_default.xml /data/local/tmp/
    adb push "<путь_к_нужной_либе>/libopencv_java4.so" /data/local/tmp/
    adb push "<путь_к_нужной_либе>/libc++_shared.so" /data/local/tmp/
    adb push <ваше_изображение>.png /data/local/tmp/test.png
    ```
4.  **Зайдите в shell устройства:**
    ```powershell
    adb shell
    ```
5.  **Внутри adb shell, перейдите в папку и запустите:**
    ```bash
    cd /data/local/tmp
    chmod +x ./face_recognition_app
    LD_LIBRARY_PATH=. ./face_recognition_app ./test.png ./output.png
    ```
6.  **Выйдите из adb shell:** (`exit`)
7.  **Скопируйте результат:**
    ```powershell
    adb pull /data/local/tmp/output.png .
    ```
    Файл `output.png` с результатом появится на вашем компьютере.
