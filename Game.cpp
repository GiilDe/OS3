
#include "Game.hpp"
#include "Thread.hpp"
#include "GameThread.hpp"
#include "utils.hpp"

/*--------------------------------------------------------------------------------
								
--------------------------------------------------------------------------------*/


/* Function sketch to use for printing the board. You will need to decide its placement and how exactly
	to bring in the field's parameters.

		cout << u8"╔" << string(u8"═") * field_width << u8"╗" << endl;
		for (uint i = 0; i < field_height ++i) {
			cout << u8"║";
			for (uint j = 0; j < field_width; ++j) {
				cout << (field[i][j] ? u8"█" : u8"░");
			}
			cout << u8"║" << endl;
		}
		cout << u8"╚" << string(u8"═") * field_width << u8"╝" << endl;
*/


Game::Game(game_params g) : m_gen_hist(), m_tile_hist(), m_threadpool() {
    interactive_on = g.interactive_on;
    print_on = g.print_on;
    m_gen_num = g.n_gen;
    filename = g.filename;
    m_thread_num = g.n_thread;

    vector<string> lines = utils::read_lines(filename);
    field_width = (int) utils::split(lines[0], ' ').size();
    field_height = (int) lines.size();
}

Game::~Game() {

}

void Game::run() {
    _init_game(); // Starts the threads and all other variables you need
    print_board("Initial Board");
    for (uint i = 0; i < m_gen_num; ++i) {
        auto gen_start = std::chrono::system_clock::now();
        _step(i); // Iterates a single generation
        auto gen_end = std::chrono::system_clock::now();
        m_gen_hist.push_back(
                (float) std::chrono::duration_cast<std::chrono::microseconds>(gen_end - gen_start).count());
        print_board(nullptr);
    } // generation loop
    print_board("Final Board");
    _destroy_game();
}

void Game::_init_game() {
    // Create threads
    // Create game fields
    // Start the threads
    // Testing of your implementation will presume all threads are started here
    vector<string> lines = utils::read_lines(filename);
    vector<vector<bool>> field;
    for (int i = 0; i < lines.size(); ++i) {
        vector<string> line = utils::split(lines[i], ' ');
        vector<bool> line_vec;
        for (string str : line) {
            int bit = stoi(str, nullptr, 10);
            bool flag = (bool) bit;
            line_vec.push_back(flag);
        }
        field.push_back(line_vec);
    }
    current_field = new GameField(field, field_width, field_height);
    next_field = new GameField(field, field_width, field_height);
    m_thread_num = std::min(m_thread_num, (uint) current_field->field.size());
    for (int i = 0; i < m_thread_num; ++i) {
        m_threadpool.push_back(new GameThread(uint(i), &jobs));
    }
//    for (int i = 0; i < m_thread_num; ++i) {
//        Thread *t = m_threadpool[i];
//        t->start();
//    }
}

void Game::_step(uint curr_gen) {
    // Push jobs to queue
    // Wait for the workers to finish calculating
    // Swap pointers between current and next field
    auto height = current_field->get_height();
    auto width = current_field->get_width();
    int job_size = height / m_thread_num;
//    vector<vector<Lock>> locks(m_thread_num - 1);
//    for(int i = 0; i < height; i++) {
//        locks.push_back(vector<Lock>());
//        for(int j = 0; j < width; j++) {
//            locks[i].push_back(Lock());
//        }
//    }
    int start = 0;
    int last = job_size - 1;
    for (int i = 0; i < m_thread_num; ++i) {
//        vector<Lock *> lower_p(width, nullptr);
//        vector<Lock *> upper_p(width, nullptr);
//        if (i != 0) { //if we are in the first job no need to lock...
//            for (int k = 0; k < width; ++k) { //...lower_p (keep them NULL)
//                lower_p[k] = &locks[i - 1][k];
//            }
//        }
        if (i == m_thread_num - 1) { //if we are in the last job no need...
            last = height - 1;       //...to lock upper_p (keep them NULL)
            //making sure the upper row of the last job is height-1
        } else {
//            for (int k = 0; k < width; ++k) {
//                upper_p[k] = &locks[i][k];
//            }
        }
        auto *j = new Job(uint(start), uint(last), current_field, next_field);
        jobs.push(j);
        start = last + 1;
        last += job_size;
    }
    for (int i = 0; i < m_thread_num; ++i) {
        Thread *t = m_threadpool[i];
        t->start();
    }
    // TODO Remove
    for (Thread *t : m_threadpool) {
        t->join();
    }
    GameField *temp = next_field;
    next_field = current_field;
    current_field = temp;
}

void Game::_destroy_game() {
    // Destroys board and frees all threads and resources
    // Not implemented in the Game's destructor for testing purposes.
    // Testing of your implementation will presume all threads are joined here
    for (Thread *t : m_threadpool) {
        t->join();
        delete t;
    }
    delete current_field;
    delete next_field;
}

const vector<float> Game::gen_hist() const {
    return m_gen_hist;
}

const vector<float> Game::tile_hist() const {
    return m_tile_hist;
}

// TODO Test
uint Game::thread_num() const {
    return (uint) m_threadpool.size();
}

void Game::print_board(const char *header) const {
    if (print_on) {

        // Clear the screen, to create a running animation
        if (interactive_on)
            system("clear");

        // Print small header if needed
        if (header != nullptr)
            cout << "<------------" << header << "------------>" << endl;

        // TODO: Print the board
        cout << u8"╔" << string(u8"═") * field_width << u8"╗" << endl;
        for (uint i = 0; i < field_height; ++i) {
            cout << u8"║";
            for (uint j = 0; j < field_width; ++j) {
                cout << ((*current_field)[i][j] ? u8"█" : u8"░");
            }
            cout << u8"║" << endl;
        }
        cout << u8"╚" << string(u8"═") * field_width << u8"╝" << endl;

        // Display for GEN_SLEEP_USEC micro-seconds on screen
        if (interactive_on)
            usleep(GEN_SLEEP_USEC);
    }
}


