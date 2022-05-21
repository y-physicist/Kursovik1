#include "general.h"
#include "Button.h"
#include "Textbox.h"
bool saveToFile = 0, loadFromFile = 0;

int main()
{
    // Разрешение экрана
    unsigned int width = 1600;
    unsigned int height = 900;

    // Создание окна
    RenderWindow window(VideoMode(width, height), "mandelbrot");
    // Лимит кадров в секунду
    window.setFramerateLimit(60);

    Texture mandelTexture;
    Sprite mandelSprite;

    // Создание белого прямоугольника - границ приближения
    RectangleShape zoomBorder(Vector2f(width / 8, height / 8));
    zoomBorder.setFillColor(Color(0, 0, 0, 0));
    zoomBorder.setOutlineColor(Color(255, 255, 255, 128));
    zoomBorder.setOutlineThickness(1.0f);
    zoomBorder.setOrigin(Vector2f(zoomBorder.getSize().x / 2, zoomBorder.getSize().y / 2));

    // Подготовка шрифта для вывода текста на экран
    Font font;
    if (!font.loadFromFile("fonts\\arial.ttf"))
        std::cout << "Font not found!\n";

    Textbox text1(20, sf::Color::White, false);

    Button btn1("Open", { 200, 50 }, 22, sf::Color::White, sf::Color::Black);
    btn1.setFont(font);
    btn1.setPosition({ 80, 800 });

    Button btn2("Save", { 200, 50 }, 22, sf::Color::White, sf::Color::Black);
    btn2.setFont(font);
    btn2.setPosition({ 330, 800 });

    double oxmin = -2.4;
    double oxmax = 1.0;
    double oyRange = (abs(oxmin) + abs(oxmax)) * height / width;
    double oymin = -oyRange / 2;
    double oymax = oyRange / 2;

    double xmin = oxmin;
    double xmax = oxmax;
    double ymin = oymin;
    double ymax = oymax;

    int recLevel = 1; // коэффициент приближения
    int precision = 64; // указание изначальной точности вычислений

    mandelTexture = mandelbrot(width, height, oxmin, oxmax, oymin, oymax, 128);

    Text zoomText, precText;
    zoomText.setFont(font);
    precText.setFont(font);
    zoomText.setFillColor(Color::White);
    precText.setFillColor(Color::White);
    zoomText.setCharacterSize(24);
    precText.setCharacterSize(24);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
        text1.setSelected(true);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        text1.setSelected(false);
    }


    while (window.isOpen())
    {
        // Отработка событий
        Event evnt;
        while (window.pollEvent(evnt))
        {
            switch (evnt.type)
            {
                // Если окно закрыто
                case Event::Closed:
                    window.close();
                    break;
                // Если клавиша отжата и это клавиша "О"
                case Event::KeyReleased:
                    if (evnt.key.code == Keyboard::Key::O)
                    {
                        // вернуться к изначальной картинке
                        recLevel = 1;
                        precision = 128;
                        loadFromFile = 0;

                        xmin = oxmin;
                        xmax = oxmax;
                        ymin = oymin;
                        ymax = oymax;
                    }
                    mandelTexture = mandelbrot(width, height, xmin, xmax, ymin, ymax, precision);  // формирование спрайта для вывода
                    break;
                // Если была замечена прокрутка мыши
                case Event::MouseWheelScrolled:
                    if (evnt.mouseWheelScroll.delta <= 0)
                    {
                        if (precision > 1)
                            precision /= 2; // то изменить точность вычислений
                        else
                            ;
                    }
                    else
                    {
                        precision *= 2; // или увеличить точность вычислений
                    }
                    mandelTexture = mandelbrot(width, height, xmin, xmax, ymin, ymax, precision);
                    break;
                case sf::Event::TextEntered:
                    text1.typedOn(evnt);
                case sf::Event::MouseMoved:
                    if (btn1.isMouseOver(window)) {
                        btn1.setBackColor(sf::Color::Magenta);
                    }
                    else {
                        btn1.setBackColor(sf::Color::White);
                    }
                    if (btn2.isMouseOver(window)) {
                        btn2.setBackColor(sf::Color::Magenta);
                    }
                    else {
                        btn2.setBackColor(sf::Color::White);
                    }
                    break;
                case sf::Event::MouseButtonPressed:
                    if (btn1.isMouseOver(window)) {
                        loadFromFile = 1;
                    }
                    if (btn2.isMouseOver(window)) {
                        saveToFile = 1;
                    }
            }
        }

        // Если нажата левая кнопка мыши
        if (Mouse::isButtonPressed(Mouse::Left) && !btn1.isMouseOver(window) && !btn2.isMouseOver(window) && !loadFromFile)
        {
            recLevel++;
            // Сформировать картину для вывода по белому прямоугольнику
            double x = zoomBorder.getPosition().x - zoomBorder.getSize().x / 2;
            double y = zoomBorder.getPosition().y - zoomBorder.getSize().y / 2;

            double x2 = x + zoomBorder.getSize().x;
            double y2 = y + zoomBorder.getSize().y;

            double normX = normalize(x, 0.0, width, xmin, xmax);
            double normY = normalize(y, 0.0, height, ymin, ymax);

            double widthNorm = normalize(x2, 0.0, width, xmin, xmax);
            double heightNorm = normalize(y2, 0.0, height, ymin, ymax);

            xmin = normX;
            xmax = widthNorm;
            ymin = normY;
            ymax = heightNorm;

            mandelTexture = mandelbrot(width, height, xmin, xmax, ymin, ymax, precision);
        }



        // Информация в левой верхней части экрана: приближение и точность
        zoomText.setString("Zoom: " + std::to_string(pow(8, recLevel-1)));
        precText.setString("Max. Iterations: " + std::to_string(precision));
        precText.setPosition(Vector2f(0, 32));

        // Формирование белого прямоугольника вокруг курсора
        zoomBorder.setPosition(Mouse::getPosition(window).x, Mouse::getPosition(window).y);

        // Формирование множества
        mandelSprite.setTexture(mandelTexture);

        // Очистка экрана (заливка белым)
        window.clear(Color::White);

        // Рисование всех изображений
        window.draw(mandelSprite);
        window.draw(zoomText);
        window.draw(precText);
        window.draw(zoomBorder);
        text1.drawTo(window);
        btn1.drawTo(window);
        btn2.drawTo(window);
        // Отображение на экране
        window.display();
        if (saveToFile) {
            Image screenshot = mandelTexture.copyToImage(); //Скрин в экземпляр класса Image
            screenshot.saveToFile("images\\image.jpg");
        }
        saveToFile = 0;

    }

    return 0;
}

double normalize(double value, double localMin, double localMax, double min, double max)
{
    // Функция нормализации цветов изображения
    double normalized = (value - localMin) / (localMax - localMin);
    normalized = normalized * (max - min);
    normalized += min;
    return normalized;
}

double mandelIter(double cx, double cy, int maxIter) { // счётчик итераций
    double x = 0.0;
    double y = 0.0;
    double xx = 0.0;
    double yy = 0.0;
    double xy = 0.0;

    double i = maxIter;
    while (i-- && xx + yy <= 4) {
        xy = x * y;
        xx = x * x;
        yy = y * y;
        x = xx - yy + cx;
        y = xy + xy + cy;
    }
    return maxIter - i;
}

Texture mandelbrot(int width, int height, double xmin, double xmax, double ymin, double ymax, int iterations)
{
    Texture texture;
    texture.create(width, height);
    Uint8* pixels = new Uint8[width * height * 4]; // количество пикселей для работы
    if (!loadFromFile) {

    for (int ix = 0; ix < width; ix++)
    {
        for (int iy = 0; iy < height; iy++)
        {
            double x = xmin + (xmax - xmin) * ix / (width - 1.0);
            double y = ymin + (ymax - ymin) * iy / (height - 1.0);

            double i = mandelIter(x, y, iterations);

            int ppos = 4 * (width * iy + ix);

            int hue = (int)(255 * i / iterations);
            int sat = 100;
            int val = (i > iterations) ? 0 : 100;
            Color hsvtorgb = HSVtoRGB(hue, sat, val); // конвертация HSV в RGB
            pixels[ppos] = (int)hsvtorgb.r;
            pixels[ppos + 1] = (int)hsvtorgb.g; //  Массив пикселей определённого
            pixels[ppos + 2] = (int)hsvtorgb.b; //  цветового диапазона
            pixels[ppos + 3] = 255;
        }
    }

    texture.update(pixels, width, height, 0, 0); // обновление пикселей

    delete[] pixels; // удаление дин. массива пикселей

        return texture;
    }
    else {
        Image mandelImage;
        mandelImage.loadFromFile("images\\image1.jpg");
        texture.loadFromImage(mandelImage);
        return texture;
    }
}

Color HSVtoRGB(float H, float S, float V)
{
    // Функция конвертации HSV в RGB
    if (H > 360 || H < 0 || S>100 || S < 0 || V > 100 || V < 0) {
        return Color::Black;
    }
    float s = S / 100;
    float v = V / 100;
    float C = s * v;
    float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    float m = v - C;
    float r, g, b;
    if (H >= 0 && H < 60) {
        r = C, g = X, b = 0;
    }
    else if (H >= 60 && H < 120) {
        r = X, g = C, b = 0;
    }
    else if (H >= 120 && H < 180) {
        r = 0, g = C, b = X;
    }
    else if (H >= 180 && H < 240) {
        r = 0, g = X, b = C;
    }
    else if (H >= 240 && H < 300) {
        r = X, g = 0, b = C;
    }
    else {
        r = C, g = 0, b = X;
    }
    int R = (r + m) * 255;
    int G = (g + m) * 255;
    int B = (b + m) * 255;
    return Color(R, G, B);
}