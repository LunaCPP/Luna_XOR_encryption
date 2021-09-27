#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(0);
    string key;
    string file_path;

    const int buffer_size = 1048576;

    cout << "Enter the file path: "; cin >> file_path;
    cout << "Enter the key: "; cin >> key;

    fstream file;
    file.open(file_path, ios::in | ios::binary | ios::ate);
    if( file.good() == true )
    {
        cout << "Successful opening of the file!" << endl;

        fstream s_file;
        string s_file_path = file_path;
        if(file_path.find(".enc") != std::string::npos)
        {
            for(int i = 0; i < 3; i++)
            {
                s_file_path.erase(s_file_path.end()-1);
            }
            s_file.open(s_file_path, ios::out | ios::binary);
        }
        else
            s_file.open(s_file_path + ".enc", ios::out | ios::binary);

        auto file_length = file.tellg();
        file.seekg (0, file.beg);
        int series = file_length / buffer_size;
        int last_buffer_size = file_length - buffer_size * series;
        cout << "buffer_size: " << buffer_size << endl << "file_length: " << file_length << endl << "series: " << series << endl << "last_buffer_size: " << last_buffer_size << endl;

        char buffor[buffer_size];
        char last_buffer[last_buffer_size];

        for(int i = 0; i < series; i++)
        {
            file.read( buffor, buffer_size );
            for(int j = 0; j < buffer_size; j++)
            {
                buffor[j] = buffor[j] ^ key[j%key.size()];
            }
            s_file.write(buffor, buffer_size);
        }

        file.read( last_buffer, last_buffer_size );
        for(int i = 0; i < last_buffer_size; i++)
        {
            last_buffer[i] = last_buffer[i] ^ key[i%key.size()];
        }
        s_file.write(last_buffer, last_buffer_size);

        file.close();
        s_file.close();
        remove(file_path.c_str());
    }
    else std::cout << "Failed to open file!" << std::endl;

    return 0;
}

