// multiple playlists using map and lists in text file
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

class Song
{
private:
    string artist;
    string title;
    double duration;
    int song_id;

public:
    Song(int song_id, const string &artist, const string &title, double duration)
    {
        this->song_id = song_id;
        this->artist = artist;
        this->title = title;
        this->duration = duration;
    }

    void update(int id, const string &a, const string &t, double d)
    {
        song_id = id;
        artist = a;
        title = t;
        duration = d;
    }

    void display_details() const
    {
        cout << "Id: " << song_id << " | "
             << "Artist: " << artist << " | "
             << "Title: " << title << " | "
             << "Duration: " << duration << " mins" << endl;
    }

    double get_duration() const { return duration; }
    int find_id() const { return song_id; }

    string to_file_format() const
    {
        stringstream ss;
        ss << song_id << "," << artist << "," << title << "," << duration;
        return ss.str();
    }

    static Song from_file_format(const string &line)
    {
        stringstream ss(line);
        string id_str, artist, title, duration_str;

        getline(ss, id_str, ',');
        getline(ss, artist, ',');
        getline(ss, title, ',');
        getline(ss, duration_str);
        //parse the string and return stuff between ','
        return Song(stoi(id_str), artist, title, stod(duration_str));//return a song object
    }
};

class Playlist
{
private:
    list<Song> songs;
    string playlist_name;

public:
    Playlist(const string &name = "") : playlist_name(name) {}

    string get_name() const { return playlist_name; }

    void add_Song(const Song &song)
    {
        if (!has_song_id(song.find_id()))
        {
            songs.push_back(song);
            cout << "Song added successfully\n";
        }
        else
        {
            cout << "Error: Song ID already exists in playlist\n";
        }
    }

    bool has_song_id(int id) const // unique id
    {
        return any_of(songs.begin(), songs.end(),[id](const Song &s){ return s.find_id() == id; });
    } // any_of is library function

    Song *find_Song(int id)
    {
        if (songs.empty())
        {
            cout << "List is empty\n";
            return nullptr;
        }
        for (auto &song : songs)
        {
            if (id == song.find_id()) // operator overloading
            {
                return &song;
            }
        }
        return nullptr;
    }
    void remove_Song(int song_id)
    {
        auto it = find_if(songs.begin(), songs.end(),[song_id](const Song &s){ return s.find_id() == song_id; });

        if (it != songs.end())
        {
            songs.erase(it);
            cout << "Song removed successfully\n";
        }
        else
        {
            cout << "Song not found\n";
        }
    }

    void display_list() const
    {
        if (songs.empty())
        {
            cout << "Playlist is empty\n";
            return;
        }
        cout << "\nPlaylist: " << playlist_name << "\n";
        cout << string(50, '-') << "\n";
        for (const auto &song : songs)
        {
            song.display_details();
        }
        cout << string(50, '-') << "\n";
    }

    void sort_Byduration()
    {
        if (songs.empty())
        {
            cout << "Playlist is empty\n";
            return;
        }
        songs.sort([](const Song &a, const Song &b){ return a.get_duration() < b.get_duration(); });
        cout << "Playlist sorted by duration\n";
    }

    void save_to_file(const string &filename)
    {
        ofstream file(filename);
        if (!file)
        {
            cout << "Error opening file for writing\n";
            return;
        }
        for (const auto &song : songs)
        {
            file << song.to_file_format() << endl;
        }
        cout << "Playlist saved to " << filename << endl;
    }

    void load_from_file(const string &filename)
    {
        ifstream file(filename);
        if (!file)
        {
            cout << "Error opening file for reading\n";
            return;
        }
        songs.clear();
        string line;
        while (getline(file, line))
        {
            songs.push_back(Song::from_file_format(line));
        }
        cout << "Playlist loaded from " << filename << endl;
    }
};

class PlaylistManager
{
private:
    map<string, Playlist> playlists;

    void display_all_playlists() const
    {
        if (playlists.empty())
        {
            cout << "No playlists exist\n";
            return;
        }
        cout << "\nExisting Playlists:\n";
        cout << string(20, '-') << "\n";
        for (const auto &pair : playlists)
        {
            cout << "- " << pair.first << "\n";
        }
        cout << string(20, '-') << "\n";
    }

    bool validate_playlist_name(const string &name) const
    {
        return !name.empty() && name.find_first_not_of(" \t\n\r") != string::npos;
    }//npos is no such character was found if sting is all whitespaces

public:
    void create_playlist()
    {
        string name;
        cout << "Current playlists:\n";
        display_all_playlists();

        cout << "Enter new playlist name:\n";
        getline(cin, name);

        if (!validate_playlist_name(name))
        {
            cout << "Error: Invalid playlist name\n";
            return;
        }

        if (playlists.find(name) != playlists.end())
        {
            cout << "Error: Playlist '" << name << "' already exists\n";
            return;
        }

        playlists[name] = Playlist(name);//playlist object passing 'name' to constructor
        cout << "Playlist '" << name << "' created successfully\n";
    }

    void add_song()
    {
        display_all_playlists();

        string playlist_name;
        cout << "Enter playlist name to add a song:\n";
        getline(cin, playlist_name);

        auto it = playlists.find(playlist_name);
        if (it == playlists.end())
        {
            cout << "Error: Playlist not found\n";
            return;
        }

        int id;
        string artist, title;
        double duration;

        cout << "Enter Song ID:\n";
        cin >> id;
        if (cin.fail())
        {
            cout << "Error: Invalid ID format\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        cin.ignore();

        if (it->second.has_song_id(id))
        {
            cout << "Error: Song ID already exists in playlist\n";
            return;
        }

        cout << "Enter Artist name:\n";
        getline(cin, artist);
        cout << "Enter Song Title:\n";
        getline(cin, title);
        cout << "Enter duration (in minutes):\n";
        cin >> duration;
        cin.ignore();

        if (duration <= 0)
        {
            cout << "Error: Duration must be positive\n";
            return;
        }

        it->second.add_Song(Song(id, artist, title, duration));
    }

    void display_playlist()
    {
        display_all_playlists();

        string name;
        cout << "Enter playlist name to display:\n";
        getline(cin, name);

        auto it = playlists.find(name);
        if (it != playlists.end())
        {
            it->second.display_list();
        }
        else
        {
            cout << "Error: Playlist not found\n";
        }
    }

    void sort_playlist()
    {
        display_all_playlists();

        string name;
        cout << "Enter playlist name to sort:\n";
        getline(cin, name);

        auto it = playlists.find(name);
        if (it != playlists.end())
        {
            it->second.sort_Byduration();
        }
        else
        {
            cout << "Error: Playlist not found\n";
        }
    }

    void update_song()
    {
        display_all_playlists();

        string playlist_name;
        cout << "Enter playlist name:\n";
        getline(cin, playlist_name);

        auto it = playlists.find(playlist_name);
        if (it == playlists.end())
        {
            cout << "Error: Playlist not found\n";
            return;
        }

        it->second.display_list();

        int id;
        cout << "Enter Song ID to update:\n";
        cin >> id;
        cin.ignore();

        Song *song = it->second.find_Song(id);
        if (!song)
        {
            cout << "Error: Song not found\n";
            return;
        }

        string artist, title;
        double duration;
        int new_id;

        cout << "Enter new Song ID (or same ID to keep):\n";
        cin >> new_id;
        cin.ignore();

        if (new_id != id && it->second.has_song_id(new_id))
        {
            cout << "Error: New ID already exists in playlist\n";
            return;
        }

        cout << "Enter new Artist name:\n";
        getline(cin, artist);
        cout << "Enter new Song Title:\n";
        getline(cin, title);
        cout << "Enter new duration:\n";
        cin >> duration;
        cin.ignore();

        if (duration <= 0)
        {
            cout << "Error: Duration must be positive\n";
            return;
        }

        song->update(new_id, artist, title, duration);
        cout << "Song updated successfully\n";
    }

    void remove_song()
    {
        display_all_playlists();

        string playlist_name;
        cout << "Enter playlist name:\n";
        getline(cin, playlist_name);

        auto it = playlists.find(playlist_name);
        if (it == playlists.end())
        {
            cout << "Error: Playlist not found\n";
            return;
        }

        it->second.display_list();

        int id;
        cout << "Enter Song ID to remove:\n";
        cin >> id;
        cin.ignore();

        it->second.remove_Song(id);
    }

    void save_playlist()
    {
        display_all_playlists();

        string playlist_name, filename;
        cout << "Enter playlist name to save:\n";
        getline(cin, playlist_name);

        auto it = playlists.find(playlist_name);
        if (it == playlists.end())
        {
            cout << "Error: Playlist not found\n";
            return;
        }

        cout << "Enter filename to save the playlist:\n";
        getline(cin, filename);

        if (filename.empty())
        {
            cout << "Error: Invalid filename\n";
            return;
        }

        it->second.save_to_file(filename);
    }

    void load_playlist()
    {
        string playlist_name, filename;
        cout << "Enter playlist name to load into:\n";
        getline(cin, playlist_name);

        if (!validate_playlist_name(playlist_name))
        {
            cout << "Error: Invalid playlist name\n";
            return;
        }

        cout << "Enter filename to load from:\n";
        getline(cin, filename);

        if (filename.empty())
        {
            cout << "Error: Invalid filename\n";
            return;
        }

        auto &playlist = playlists[playlist_name];
        playlist.load_from_file(filename);
    }
};

int main()
{
    PlaylistManager manager;
    int choice;
    cout << "\nPlaylist Management System\n";
    cout << string(25, '=') << "\n";
    cout << "1: Create New Playlist\n"
         << "2: Add Song to Playlist\n"
         << "3: Display Playlist\n"
         << "4: Sort Playlist by Duration\n"
         << "5: Update Song Details\n"
         << "6: Save Playlist to File\n"
         << "7: Remove Song from Playlist\n"
         << "8: Load Playlist from File\n"
         << "9: Exit\n";
    cout << string(25, '=') << "\n";
    while (true)
    {

        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore();

        switch (choice)
        {
        case 1:
            manager.create_playlist();
            break;
        case 2:
            manager.add_song();
            break;
        case 3:
            manager.display_playlist();
            break;
        case 4:
            manager.sort_playlist();
            break;
        case 5:
            manager.update_song();
            break;
        case 6:
            manager.save_playlist();
            break;
        case 7:
            manager.remove_song();
            break;
        case 8:
            manager.load_playlist();
            break;
        case 9:
            cout << "Thank you for using Playlist Manager!\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}
