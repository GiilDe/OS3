
#include "Game.hpp"
#include "Thread.hpp"
#include "GameThread.hpp"
#include "utils.hpp"
#include "TerminatingJob.hpp"

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


Game::Game(game_params g) :
        m_gen_hist(),
        m_tile_hist(),
        m_threadpool(),
        interactive_on(g.interactive_on),
        print_on(g.print_on),
        m_gen_num(g.n_gen),
        filename(g.filename),
        m_thread_num(g.n_thread),
        tile_lock() {
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
    bool_mat field;
    vector<string> lines = utils::read_lines(filename);
    for (const string& str_line : lines) {
        vector<string> line = utils::split(str_line, ' ');
        vector<bool> line_vec;
        for (const string& str : line) {
            line_vec.push_back((bool) stoi(str, nullptr, 10));
        }
        field.push_back(line_vec);
    }
    field_width = (int) utils::split(lines[0], ' ').size();
    field_height = (int) lines.size();

    current_field = new bool_mat(field);
    next_field = new bool_mat((uint)field_height, vector<bool>((uint)field_width));

    for (int i = 0; i < thread_num(); ++i) {
        m_threadpool.push_back(new GameThread(uint(i), &jobs, &tile_lock, &m_tile_hist));
    }
    start_all_threads();
}

void Game::start_all_threads() {
    for (int i = 0; i < thread_num(); ++i) {
        Thread *t = m_threadpool[i];
        t->start();
    }
}

void Game::wait_for_threads() {

}

void Game::_step(uint curr_gen) {
    // Push jobs to queue
    // Wait for the workers to finish calculating
    // Swap pointers between current and next field
    int job_size = (uint)field_height / thread_num();

    uint start = 0;
    uint last = (uint)job_size - 1;

    for (int i = 0; i < thread_num(); ++i) {
        if (i == thread_num() - 1) {
            last = (uint)field_height - 1;
        }
        auto *j = new Job(uint(start), uint(last), current_field, next_field);
        jobs.push(j);
        start = last + 1;
        last += job_size;
    }

    // Wait for the job queue to empty
    wait_for_threads();
    while(!jobs.empty());

    // Swap the current and next boards
    bool_mat *temp = next_field;
    next_field = current_field;
    current_field = temp;

    // Push thread_num() jobs to the queue, which signal the threads that they should terminate
    // This is because we want the threads to stop working when m_gen_num generations are exceeded
    if(curr_gen == m_gen_num - 1) {
        for (int i = 0; i < thread_num(); ++i) {
            jobs.push(new TerminatingJob());
        }
    }
}

void Game::_destroy_game() {
    // Destroys board and frees all threads and resources
    // Not implemented in the Game's destructor for testing purposes.
    // Testing of your implementation will presume all threads are joined here
    for(Thread *t : m_threadpool) {
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
    return std::min(m_thread_num, (uint) field_height);
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


