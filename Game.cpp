
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


Game::Game(game_params g): m_gen_hist(), m_tile_hist(), m_threadpool() {
    interactive_on = g.interactive_on;
    print_on = g.print_on;
    m_gen_num = g.n_gen;
    filename = g.filename;
    m_thread_num = g.n_thread;

    vector<string> lines = utils::read_lines(filename);
    field_height = (int) utils::split(lines[0], ' ').size();
    field_width = (int) lines.size();
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
    current_field = new GameField(utils::read_lines(filename));
    next_field = new GameField(utils::read_lines(filename));
    m_thread_num = std::min(m_thread_num, (uint) current_field->field.size());
    for (int i = 0; i < m_thread_num; ++i) {
        m_threadpool.push_back(new GameThread(uint(i), &jobs));
    }
    for (int i = 0; i < m_thread_num; ++i) {
        Thread *t = m_threadpool[i];
        t->start();
    }
}

void Game::_step(uint curr_gen) {
    // Push jobs to queue
    // Wait for the workers to finish calculating
    // Swap pointers between current and next field
    int height = current_field->field.size();
    int job_size = height / m_thread_num;
    vector<Lock> locks(m_thread_num - 1, Lock());
    for (int i = 0; i < m_thread_num; ++i) {
        Lock *lower_p;
        Lock *upper_p;
        if (i == 0) {
            lower_p = NULL;
        } else {
            lower_p = &locks[i - 1];
        }
        int last = i + job_size;
        if (i == m_thread_num - 1) {
            last = height - 1;
            upper_p = NULL;
        } else {
            upper_p = &locks[i];
        }
        Job* j = new Job(uint(i), uint(last), current_field, next_field, lower_p, upper_p);
        jobs.push(j);
    }
    for (int i = 0; i < m_thread_num; ++i) {
        Thread *t = m_threadpool[i];
        t->start();
    }
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


