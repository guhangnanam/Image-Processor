#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

struct Pixel{
public:
    unsigned char blue;
    unsigned char green;
    unsigned char red;

    Pixel(unsigned char _blue, unsigned char _green, unsigned char _red){    // Parameterized constructor for each individual pixel
        blue = _blue;
        green = _green;
        red = _red;
    }

    Pixel(){
        blue = 0;
        green = 0;
        red = 0;
    }

    unsigned char GetBlue(){
        return blue;
    }

    unsigned char GetRed(){
        return red;
    }

    unsigned char GetGreen(){
        return green;
    }

};
class Image {
public:

    struct Header {
        char idLength;
        char colorMapType;
        char dataTypeCode;
        short colorMapOrigin;
        short colorMapLength;
        char colorMapDepth;
        short xOrigin;
        short yOrigin;
        short width;
        short height;
        char bitsPerPixel;
        char imageDescriptor;
        vector<Pixel> pixels;
    };
    Header header_obj;
    // Constructors
    Image(Header header_data){        // parametrized constructor for an individual image
        Header header;
        header.idLength = header_data.idLength;
        header.colorMapType = header_data.colorMapType;
        header.dataTypeCode = header_data.dataTypeCode;
        header.colorMapOrigin = header_data.colorMapOrigin;
        header.colorMapLength = header_data.colorMapLength;
        header.colorMapDepth = header_data.colorMapDepth;
        header.xOrigin = header_data.xOrigin;
        header.yOrigin = header_data.yOrigin;
        header.width = header_data.width;
        header.height = header_data.height;
        header.bitsPerPixel = header_data.bitsPerPixel;
        header.imageDescriptor = header_data.imageDescriptor;
        this->header_obj = header;
    }

    // Getters
    Header GetHeaderData(){
        return header_obj;
    }

    Header GetHeaderData() const{
        return header_obj;
    }

    unsigned int GetPixelVectorSize(){
        return header_obj.pixels.size();
    }

    unsigned int GetPixelVectorSize() const{
        return header_obj.pixels.size();
    }

    Pixel GetSpecificPixel(int index){
        return header_obj.pixels.at(index);
    }

    Pixel GetSpecifcPixel(int index) const{
        return header_obj.pixels.at(index);
    }
};


float Normalize(float input_color, float top_pixel, float bot_pixel){
    float temp = ((float)input_color / (float)( top_pixel - bot_pixel));
    return temp;
}

// Read a file
Image Read(string file_name){
    ifstream image(file_name, ios_base::binary);

    if (image.is_open()){
        // Pass in and assign all image format data to their correct variables
        Image::Header header_data;
        image.read(&header_data.idLength, sizeof(header_data.idLength));
        image.read(&header_data.colorMapType, sizeof(header_data.colorMapType));
        image.read(&header_data.dataTypeCode, sizeof(header_data.dataTypeCode));
        image.read((char*)&header_data.colorMapOrigin, sizeof(header_data.colorMapOrigin));
        image.read((char*)&header_data.colorMapLength, sizeof(header_data.colorMapLength));
        image.read(&header_data.colorMapDepth, sizeof(header_data.colorMapDepth));
        image.read((char*)&header_data.xOrigin, sizeof(header_data.xOrigin));
        image.read((char*)&header_data.yOrigin, sizeof(header_data.yOrigin));
        image.read((char*)&header_data.width, sizeof(header_data.width));
        image.read((char*)&header_data.height, sizeof(header_data.height));
        image.read(&header_data.bitsPerPixel, sizeof(header_data.bitsPerPixel));
        image.read(&header_data.imageDescriptor, sizeof(header_data.imageDescriptor));

        // Pass in and assign all pixels to the vector of Pixels
        int resolution = header_data.width * header_data.height;



        vector<Pixel> pixels;
        for (int i = 0; i < resolution; ++i){
            unsigned char _red;      // create unsigned char variables
            unsigned char _green;
            unsigned char _blue;
            image.read((char*)&_blue, sizeof(_blue));           // read the pixel color data for each color
            image.read((char*)&_green, sizeof(_green));
            image.read((char*)&_red, sizeof(_red));

            Pixel new_pixel(_blue, _green, _red);         // Call constructor for Pixel struct
            pixels.push_back(new_pixel);                   // push back new_pixel into vector of Pixels
        }

        // Create new image with header data now collected
        Image new_image = Image(header_data);
        new_image.header_obj.pixels = pixels;

        return new_image;
    }
}

// Write a file
void Write(string file_name, Image& new_image){
    ofstream image(file_name, ios_base::binary);


    if(image.is_open()){
        // Write Header Struct data
        Image::Header header_data = new_image.GetHeaderData();
        image.write(&header_data.idLength, sizeof(header_data.idLength));
        image.write(&header_data.colorMapType, sizeof(header_data.colorMapType));
        image.write(&header_data.dataTypeCode, sizeof(header_data.dataTypeCode));
        image.write((char*)&header_data.colorMapOrigin, sizeof(header_data.colorMapOrigin));
        image.write((char*)&header_data.colorMapLength, sizeof(header_data.colorMapLength));
        image.write(&header_data.colorMapDepth, sizeof(header_data.colorMapDepth));
        image.write((char*)&header_data.xOrigin, sizeof(header_data.xOrigin));
        image.write((char*)&header_data.yOrigin, sizeof(header_data.yOrigin));
        image.write((char*)&header_data.width, sizeof(header_data.width));
        image.write((char*)&header_data.height, sizeof(header_data.height));
        image.write(&header_data.bitsPerPixel, sizeof(header_data.bitsPerPixel));
        image.write(&header_data.imageDescriptor, sizeof(header_data.imageDescriptor));

        // Write the pixels from passed in image into new image
        for (unsigned int i = 0; i < new_image.GetPixelVectorSize(); ++i){

            Pixel new_pixel = new_image.GetSpecificPixel(i);   // Iterate through each pixel in the image

            unsigned char r;
            unsigned char g;
            unsigned char b;

            r = new_pixel.GetRed();             // find the red green and blue color values for the specfic pixel
            g = new_pixel.GetGreen();
            b = new_pixel.GetBlue();

            image.write((char*)&b, sizeof(b));          // write in the separate color values for the pixel
            image.write((char*)&g, sizeof(g));
            image.write((char*)&r, sizeof(r));

        }

        image.close();
    }

}

// Tasks

Image Multiply(const Image& P1, const Image& P2){

    Image new_image = Image(P1.GetHeaderData());

    for(int i = 0; i < P1.GetPixelVectorSize(); ++i) {
        Pixel top_pixel = P1.GetSpecifcPixel(i);
        Pixel bot_pixel = P2.GetSpecifcPixel(i);

        Pixel p;
        p.blue = (unsigned char)(Normalize((float)top_pixel.GetBlue() * (float)bot_pixel.GetBlue(), 255.0f, 0.0f) + 0.5f);
        p.green = (unsigned char)(Normalize((float)top_pixel.GetGreen() * (float)bot_pixel.GetGreen(), 255.0f, 0.0f) + 0.5f);
        p.red = (unsigned char)(Normalize((float)top_pixel.GetRed() * (float)bot_pixel.GetRed(), 255.0f, 0.0f) + 0.5f);



        new_image.header_obj.pixels.push_back(p);

    }

    return new_image;

};

Image Subtraction(const Image& P1, const Image& P2){
    Image new_image = Image(P1.GetHeaderData());

    for (int i = 0; i < P1.GetPixelVectorSize(); ++i){
        Pixel top_pixel = P1.GetSpecifcPixel(i);
        Pixel bot_pixel = P2.GetSpecifcPixel(i);

        // Get color values for pixels
        int top_b = (int)top_pixel.GetBlue();
        int top_g = (int)top_pixel.GetGreen();
        int top_r = (int)top_pixel.GetRed();

        int bot_b = (int)bot_pixel.GetBlue();
        int bot_g = (int)bot_pixel.GetGreen();
        int bot_r = (int)bot_pixel.GetRed();

        // Add pixel
        unsigned char new_blue;
        int temp_blue = top_b - bot_b;
        if (temp_blue < 0){
            new_blue = 0;
        }
        else{
            new_blue = temp_blue;
        }

        unsigned char new_green;
        int temp_green = top_g - bot_g;

        if (temp_green < 0){
            new_green = 0;
        }
        else{
            new_green = temp_green;
        }

        unsigned char new_red;
        int temp_red = top_r - bot_r;

        if (temp_red < 0){
            new_red = 0;
        }
        else{
            new_red = temp_red;
        }

        Pixel new_pixel = Pixel(new_blue, new_green, new_red);
        new_image.header_obj.pixels.push_back(new_pixel);

    }
    return new_image;
};

Image AddColor(const Image& P1, int blue, int green, int red){

    Image new_image = Image(P1.GetHeaderData());

    for (int i = 0; i < P1.GetPixelVectorSize(); i++){
        Pixel temp_pixel = P1.GetSpecifcPixel(i);

        int b = (int)temp_pixel.GetBlue();
        int g = (int)temp_pixel.GetGreen();
        int r = (int)temp_pixel.GetRed();

        int temp_b = b + blue;
        int temp_g = g + green;
        int temp_r = r + red;

        unsigned char new_blue;
        unsigned char new_green;
        unsigned char new_red;

        if (temp_b > 255){
            new_blue = 255;
        }
        else if (temp_b < 0){
            new_blue = 0;
        }
        else{
            new_blue = (unsigned char) temp_b;
        }
        if (temp_g > 255){
            new_green = 255;
        }
        else if (temp_g < 0){
            new_green = 0;
        }
        else{
            new_green = (unsigned char) temp_g;
        }
        if (temp_r > 255){
            new_red = 255;
        }
        else if (temp_r < 0){
            new_red = 0;
        }
        else{
            new_red = (unsigned char) temp_r;
        }

        Pixel new_pixel = Pixel(new_blue, new_green, new_red);
        new_image.header_obj.pixels.push_back(new_pixel);

    }

    return new_image;

};

Image Screen(const Image& P1, const Image& P2){

    Image new_image = Image(P1.GetHeaderData());

    for (int i = 0; i < P1.GetPixelVectorSize(); ++i){
        Pixel top_pixel = P1.GetSpecifcPixel(i);
        Pixel bot_pixel = P2.GetSpecifcPixel(i);

        float top_b = (float) (Normalize(top_pixel.GetBlue(), 255, 0));
        float top_g = (float) (Normalize(top_pixel.GetGreen(), 255, 0));
        float top_r = (float) (Normalize(top_pixel.GetRed(), 255, 0));

        float bot_b = (float) (Normalize(bot_pixel.GetBlue(), 255, 0));
        float bot_g = (float) (Normalize(bot_pixel.GetGreen(), 255, 0));
        float bot_r = (float) (Normalize(bot_pixel.GetRed(), 255, 0));

        float temp_b = 1.0f - ((1.0f - top_b) * (1.0f - bot_b));
        float temp_g = 1.0f - ((1.0f - top_g) * (1.0f - bot_g));
        float temp_r = 1.0f - ((1.0f - top_r) * (1.0f - bot_g));

        unsigned char new_blue = (unsigned char)(temp_b * 255 + 0.5f);
        unsigned char new_green = (unsigned char)(temp_g * 255 + 0.5f);
        unsigned char new_red = (unsigned char)(temp_r * 255 + 0.5f);

        Pixel new_pixel = Pixel(new_blue, new_green, new_red);
        new_image.header_obj.pixels.push_back(new_pixel);

    }

    return new_image;
};

Image Overlay(const Image& P1, const Image& P2){
    Image new_image = Image(P1.GetHeaderData());

    for (int i = 0; i < P1.GetPixelVectorSize(); ++i){      // Iterate through the Pixel vector
        Pixel top_pixel = P1.GetSpecifcPixel(i);            // Assign top and bottom pixel
        Pixel bot_pixel = P2.GetSpecifcPixel(i);

        // Assign color values for top and bottom pixel and Normalize them
        float top_b = (Normalize(top_pixel.GetBlue(), 255, 0));
        float top_g = (Normalize(top_pixel.GetGreen(), 255, 0));
        float top_r = (Normalize(top_pixel.GetRed(), 255, 0));

        float bot_b = (Normalize(bot_pixel.GetBlue(), 255, 0));
        float bot_g = (Normalize(bot_pixel.GetGreen(), 255, 0));
        float bot_r = (Normalize(bot_pixel.GetRed(), 255, 0));

        unsigned char new_blue;
        float temp_b;
        if (bot_b <= 0.5){        // check if P2 color value is less than or equal to 0.5
            temp_b = 2 * top_b * bot_b;     // Set up temp variable
            if (temp_b > 1){            // check if temp is greater than 1
                new_blue = 255;
            }
            else{                               // create new_blue value
                new_blue = (unsigned char)(temp_b * 255 + 0.5f);
            }
        }
        else{                       // check if P2 color value is greater than 0.5
            temp_b = 1 - 2 * (1 - top_b) * (1 - bot_b);  // Set up temp variable
            if(temp_b > 1){     // check if temp is greater than 1
                new_blue = 255;
            }
            else if (temp_b < 0){       // check if temp is less than 0
                new_blue = 0;
            }
            else {
                new_blue = (unsigned char) (temp_b * 255 + 0.5f); // create new blue variable
            }
        }

        unsigned char new_green;
        float temp_g;
        if (bot_g <= 0.5){
            temp_g = 2 * top_g * bot_g;
            if (temp_g > 1){
                new_green = 255;
            }
            else{
                new_green = (unsigned char)(temp_g * 255 + 0.5f);
            }
        }
        else{
            temp_g = 1 - 2 * (1 - top_g) * (1 - bot_g);
            if (temp_g > 1){
                new_green = 255;
            }
            else if (temp_g < 0){
                new_green = 0;
            }
            else{
            new_green = (unsigned char)(temp_g * 255 + 0.5f);
            }
        }

        unsigned char new_red;
        float temp_r;
        if (bot_r <= 0.5){
            temp_r = 2 * top_r * bot_r;
            if (temp_r > 1){
                new_red = 255;
            }
            else{
                new_red = (unsigned char)(temp_r * 255 + 0.5f);
            }
        }
        else{
            temp_r = 1 - 2 * (1 - top_r) * (1 - bot_r);
            if (temp_r > 1){
                new_red = 255;
            }
            else if (temp_r < 0){
                new_red = 0;
            }
            else{
                new_red = (unsigned char)(temp_r * 255 + 0.5f);
            }
        }

        Pixel new_pixel = Pixel(new_blue , new_green, new_red);     // create new pixel and push it back
        new_image.header_obj.pixels.push_back(new_pixel);

    }
    return new_image;
}

Image MultiplyColor(const Image& P1, float blue_scale, float green_scale, float red_scale){

    Image new_image = Image(P1.GetHeaderData());

    for (int i = 0; i < P1.GetPixelVectorSize(); i++){
        Pixel temp_pixel = P1.GetSpecifcPixel(i);

        float b = (float)temp_pixel.GetBlue();
        float g = (float)temp_pixel.GetGreen();
        float r = (float)temp_pixel.GetRed();

        int temp_b = b * blue_scale;
        int temp_g = g * green_scale;
        int temp_r = r * red_scale;

        unsigned char new_blue;
        unsigned char new_green;
        unsigned char new_red;

        if (temp_b > 255){
            new_blue = (unsigned char) 255;
        }
        else{
            new_blue = (unsigned char) temp_b;
        }
        if (temp_g > 255){
            new_green = (unsigned char) 255;
        }
        else{
            new_green = (unsigned char) temp_g;
        }
        if (temp_r > 255){
            new_red = (unsigned char) 255;
        }
        else{
            new_red = (unsigned char) temp_r;
        }

        Pixel new_pixel = Pixel(new_blue, new_green, new_red);
        new_image.header_obj.pixels.push_back(new_pixel);

    }

    return new_image;

};

vector<Image> SeparateColor(const Image& P1){
    vector<Image> layers;
    layers.push_back(Image(P1.GetHeaderData()));      // Blue Layer
    layers.push_back(Image(P1.GetHeaderData()));       // Green Layer
    layers.push_back(Image(P1.GetHeaderData()));        // Red layer

    for (int i = 0; i < P1.GetPixelVectorSize(); ++i){
        Pixel temp_pixel = P1.GetSpecifcPixel(i);

        unsigned char blue_pixel = temp_pixel.GetBlue();
        unsigned char green_pixel = temp_pixel.GetGreen();
        unsigned char red_pixel = temp_pixel.GetRed();

        Pixel blue_layer = Pixel(blue_pixel, blue_pixel, blue_pixel);
        Pixel green_layer = Pixel(green_pixel, green_pixel, green_pixel);
        Pixel red_layer = Pixel(red_pixel, red_pixel, red_pixel);

        layers.at(0).header_obj.pixels.push_back(blue_layer);
        layers.at(1).header_obj.pixels.push_back(green_layer);
        layers.at(2).header_obj.pixels.push_back(red_layer);
    }

    return layers;
}

Image Combine(const Image& blue, const Image& green, const Image& red){       // Pass in blue, green, red
    Image new_image = Image(blue.GetHeaderData());

    for (int i = 0; i < blue.GetPixelVectorSize(); ++i){
        unsigned char blue_pixel = blue.GetSpecifcPixel(i).GetBlue();
        unsigned char green_pixel = green.GetSpecifcPixel(i).GetGreen();
        unsigned char red_pixel = red.GetSpecifcPixel(i).GetRed();

        Pixel new_pixel = Pixel(blue_pixel, green_pixel, red_pixel);
        new_image.header_obj.pixels.push_back(new_pixel);
    }

    return new_image;
};

Image Rotate(const Image& P1){
    Image new_image = Image(P1.GetHeaderData());

    for (int i = (P1.GetPixelVectorSize() - 1); i > -1; --i){
        Pixel temp_pixel = P1.GetSpecifcPixel(i);
        new_image.header_obj.pixels.push_back(temp_pixel);
    }
    return new_image;
}


int main(int argc, char* argv[]){

    // Task 1
    Image layer1 = Read("input/layer1.tga");
    Image pattern1 = Read("input/pattern1.tga");
    Image result1 = Multiply(layer1, pattern1);
    Write("output/outputTask1.tga", result1);

    // Task 2
    Image car = Read("input/car.tga");
    Image layer2 = Read("input/layer2.tga");
    Image result2 = Subtraction(car, layer2);
    Write("output/outputTask2.tga", result2);

    // Task 3
    Image pattern2 = Read("input/pattern2.tga");
    Image text = Read("input/text.tga");
    Image temp3 = Multiply(layer1, pattern2);
    Image result3 = Screen(temp3, text);
    Write("output/outputTask3.tga", result3);

    // Task 4
    Image circles= Read("input/circles.tga");
    Image temp4 = Multiply(layer2, circles);
    Image result4 = Subtraction(temp4, pattern2);
    Write("output/outputTask4.tga", result4);

    // Task 5
    Image result5 = Overlay(layer1, pattern1);
    Write("output/outputTask5.tga", result5);

    // Task 6
    Image result6 = AddColor(car, 0, 200, 0);
    Write("output/outputTask6.tga", result6);

    // Task 7
    Image result7 = MultiplyColor(car, 0, 1, 4);
    Write("output/outputTask7.tga", result7);

    // Task 8
    vector<Image> result8 = SeparateColor(car);
    Image blue_layer = result8[0];
    Image green_layer = result8[1];
    Image red_layer = result8[2];

    Write("output/part8_r.tga", red_layer);
    Write("output/part8_g.tga", green_layer);
    Write("output/part8_b.tga", blue_layer);

    // Task 9
    Image layer_red = Read("input/layer_red.tga");
    Image layer_blue = Read("input/layer_blue.tga");
    Image layer_green = Read("input/layer_green.tga");

    Image result9 = Combine(layer_blue, layer_green, layer_red);
    Write("output/outputTask9.tga", result9);

    // Task 10
    Image text2 = Read("input/text2.tga");
    Image result10 = Rotate(text2);
    Write("output/outputTask10.tga", result10);


    if (argc < 2 or string(argv[1]) == "--help"){
        // Print name of the Project and help message
        cout << "Project 2: Image Processing, Spring 2023"
        << "\n\nUsage:\n\t./project2.out [output] [firstImage] [method] [...]" << endl;

        return 0;
    }
    string output = string(argv[1]);
    string image_first;
    try {
        string file_extenstion = string(argv[1]).substr(string(argv[1]).length() - 4);
    }
    catch (std::out_of_range){
        cout << "Invalid file name." << endl;
        return 0;
    }





    if (argc < 2 or string(argv[1]).substr(string(argv[1]).length() - 4) != ".tga"){
        cout << "Invalid file name." << endl;
        return 0;
    }

    if (output == "out"){
        cout << "Invalid file name." << endl;
        return 0;
    }


    string file_extenstion2 = string(argv[2]).substr(string(argv[2]).length() - 4);
    ifstream file(argv[2], ios_base::binary);

    if (file_extenstion2 != ".tga"){
        cout << "Invalid file name." << endl;
        return 0;
    }

    if (!file.is_open()){
        cout << "File does not exist." << endl;
        return 0;
    }



    image_first = string(argv[2]);
    Image image1 = Read(image_first);



    int index_counter = 3;
    while(argc > index_counter) {
        string method_choice = string(argv[index_counter]);
        index_counter++;



        if (method_choice == "multiply") {      // if choice is multiply

            if (argc < index_counter + 1){
                cout << "Missing argument." << endl;
                return 0;
            }

            string file_extenstion3 = string(argv[index_counter]).substr(string(argv[index_counter]).length() - 4);
            ifstream temp_file1(string(argv[index_counter]), ios_base::binary);

            if (argc < index_counter + 1 or file_extenstion3 != ".tga") {             // check if file is the right extension
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }

            else if (!temp_file1.is_open()) {
                cerr << "Invalid argument, file does not exist." << endl;
                return 0;
            }

            string image_second = string(argv[index_counter]);    // create string file_name for second image
            Image image2 = Read(image_second);      // create Image object for second image

            Image result1 = Multiply(image1, image2);      // create Image object for result of multiplication
            Write(output,result1);             // Write an image file with the name of the output using the result Image
            index_counter++;

            Image common_result = result1;



        }

        else if (method_choice == "combine") {

            if (argc < index_counter + 1 or
                string(argv[index_counter]).substr(string(argv[index_counter]).length() - 4) != ".tga") {
                cout << "Invalid file name." << endl;
                return 1;
            }
            ifstream temp_file2(string(argv[index_counter]), ios_base::binary);
            if (!temp_file2.is_open()) {
                cout << "File does not exist." << endl;
                return 1;
            }
            if (argc < index_counter + 2 or
                string(argv[index_counter + 1]).substr(string(argv[index_counter + 1]).length() - 4) != ".tga") {
                cout << "Invalid file name." << endl;
                return 1;
            }
            ifstream temp_file3(string(argv[index_counter + 1]), ios_base::binary);
            if (!temp_file3.is_open()) {
                cout << "Invalid argument, file does not exist." << endl;
                return 1;
            }

            string image_second = string(argv[index_counter]);
            string image_third = string(argv[index_counter + 1]);
            Image image2 = Read(image_second);
            Image image3 = Read(image_third);

            Image result2 = Combine(image3, image2, image1);
            Write(output, result2);
            index_counter++;



        }

        else if (method_choice == "subtract") {
            if (argc < index_counter + 1 or
                string(argv[index_counter]).substr(string(argv[index_counter]).length() - 4) != ".tga") {
                cout << "Invalid file name." << endl;
                return 0;
            }
            ifstream temp_file5(string(argv[index_counter]), ios_base::binary);
            if (temp_file5.is_open() == false) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }

            string image_second = string(argv[index_counter]);
            Image image2 = Read(image_second);

           // if(argc > 4){
                //Image result3 = Sub
            //}


            Image result3 = Subtraction(image1, image2);
            Write(output, result3);
            index_counter++;


        }
        else if (method_choice == "screen") {
            if (argc < index_counter + 1 or
                string(argv[index_counter]).substr(string(argv[index_counter]).length() - 4) != ".tga") {
                cout << "Invalid file name." << endl;
                return 0;
            }
            ifstream temp_file6(string(argv[index_counter]), ios_base::binary);
            if (!temp_file6.is_open()) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }

            string image_second = string(argv[index_counter]);
            Image image2 = Read(image_second);
            Image result4 = Screen(image1, image2);
            Write(output, result4);
            index_counter++;
        }
        else if (method_choice == "overlay") {
            if (argc < index_counter + 1 or
                string(argv[index_counter]).substr(string(argv[index_counter]).length() - 4) != ".tga") {
                cout << "Invalid file name." << endl;
            }
            ifstream temp_file6(string(argv[index_counter]), ios_base::binary);
            if (!temp_file6.is_open()) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }

            string image_second = string(argv[index_counter]);
            Image image2 = Read(image_second);
            Image result5 = Overlay(image1, image2);
            Write(output, result5);
            index_counter++;

        }
        else if (method_choice == "flip") {

                Image result6 = Rotate(image1);
                Write(output, result6);
                //index_counter++;

        }

        else if (method_choice == "onlyred") {
            vector<Image> colors = SeparateColor(image1);
            Image result7 = colors[2];
            Write(output, result7);
            index_counter++;
        }
        else if (method_choice == "onlygreen") {
            vector<Image> colors = SeparateColor(image1);
            Image result8 = colors[1];
            Write(output, result8);
            index_counter++;
        }
        else if (method_choice == "onlyblue") {
            vector<Image> colors = SeparateColor(image1);
            Image result9 = colors[0];
            Write(output, result9);
            index_counter++;
        }
        else if (method_choice == "addred") {
            if (argc < index_counter + 1){
                cout << "Missing argument." << endl;
                return 0;
            }

            try {
                stoi(string(argv[index_counter]));
            }
            catch (invalid_argument) {
                cout << "Invalid argument, expected number." << endl;
        }
            ifstream temp_file7(string(argv[2]), ios_base::binary);
            if (!temp_file7.is_open()){
                cout << "File does not exist." << endl;
            }

            string red_amount = string(argv[index_counter]);
            int add_red = stoi(red_amount);
            Image result10 = AddColor(image1, 0, 0, add_red);
            Write(output, result10);
            index_counter++;
        }

        else if (method_choice == "addblue") {
            if (argc < index_counter + 1){
                cout << "Missing argument." << endl;
                return 0;
            }

            try {
                stoi(string(argv[index_counter]));
            }
            catch (invalid_argument) {
                cout << "Invalid argument, expected number." << endl;
            }
            ifstream temp_file8(string(argv[2]), ios_base::binary);
            if (!temp_file8.is_open()){
                cout << "File does not exist." << endl;
            }

            string blue_amount = string(argv[index_counter]);
            int add_blue = stoi(blue_amount);
            Image result11 = AddColor(image1, add_blue, 0, 0);
            Write(output, result11);
            index_counter++;
        }

        else if (method_choice == "addgreen") {
            if (argc < index_counter + 1){
                cout << "Missing argument." << endl;
                return 0;
            }

            try {
                stoi(string(argv[index_counter]));
            }
            catch (invalid_argument) {
                cout << "Invalid argument, expected number." << endl;
            }
            ifstream temp_file9(string(argv[2]), ios_base::binary);
            if (!temp_file9.is_open()){
                cout << "File does not exist." << endl;
            }

            string green_amount = string(argv[index_counter]);
            int add_green = stoi(green_amount);
            Image result12 = AddColor(image1, 0, add_green, 0);
            Write(output, result12);
            index_counter++;
        }

        else if (method_choice == "scalered"){
            if (argc < index_counter + 1){
                cout << "Missing argument." << endl;
                return 0;
            }

            try{
                stoi(string(argv[index_counter]));
            }
            catch (invalid_argument){
                cout << "Invalid argument, expected number." << endl;
            }

            ifstream temp_file10(string(argv[2]), ios_base::binary);
            if (!temp_file10.is_open()){
                cout << "File does not exist." << endl;
                return 0;
            }

            string red_amount = string(argv[index_counter]);
            float scale_red = (float)(stoi(red_amount));
            Image result13 = MultiplyColor(image1, 1, 1, scale_red);
            Write(output, result13);
            index_counter++;
        }

        else if (method_choice == "scalegreen"){
            if (argc < index_counter + 1){
                cout << "Missing argument." << endl;
                return 0;
            }

            try{
                stoi(string(argv[index_counter]));
            }
            catch (invalid_argument){
                cout << "Invalid argument, expected number." << endl;
                return 0;
            }

            ifstream temp_file11(string(argv[2]), ios_base::binary);
            if (!temp_file11.is_open()){
                cout << "File does not exist." << endl;
                return 0;
            }

            string green_amount = string(argv[index_counter]);
            int scale_green = stoi(green_amount);
            Image result14 = MultiplyColor(image1, 1, scale_green, 1);
            Write(output, result14);
            index_counter++;
        }

        else if (method_choice == "scaleblue"){

            if (argc < index_counter + 1){
                cout << "Missing argument." << endl;
                return 0;
            }

            try{
                stoi(string(argv[index_counter]));
            }
            catch (invalid_argument){
                cout << "Invalid argument, expected number." << endl;
                return 0;
            }

            ifstream temp_file12(string(argv[2]), ios_base::binary);
            if (!temp_file12.is_open()){
                cout << "File does not exist." << endl;
                return 0;
            }

            string blue_amount = string(argv[index_counter]);
            float scale_blue = (float)(stoi(blue_amount));
            Image result15 = MultiplyColor(image1, scale_blue, 1, 1);
            Write(output, result15);
            index_counter++;
        }

        else{
            cout << "Invalid method name." << endl;
        }


        image1 = Read(output);

    }
    return 0;
    }

