
#include "Game.hpp"
#include "Thread.hpp"
#include "GameThread.hpp"
#include "utils.hpp"

/*--------------------------------------------------------------------------------
								
--------------------------------------------------------------------------------*/
inline static void print_board(const char* header) {
    if(print_on) {

        // Clear the screen, to create a running animation
        if(interactive_on)
            system("clear");

        // Print small header if needed
        if (header != NULL)
            cout << "<------------" << header << "------------>" << endl;

        // TODO: Print the board

        // Display for GEN_SLEEP_USEC micro-seconds on screen
        if(interactive_on)
            usleep(GEN_SLEEP_USEC);
    }
}


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


Game::Game(game_params g){
    interactive_on = g.interactive_on;
    print_on = g.print_on;
    m_gen_num = g.n_gen;
    filename = g.filename;
    m_thread_num = g.n_thread;
}

void Game::run() {
	_init_game(); // Starts the threads and all other variables you need
	print_board("Initial Board");
	for (uint i = 0; i < m_gen_num; ++i) {
		auto gen_start = std::chrono::system_clock::now();
		_step(i); // Iterates a single generation 
		auto gen_end = std::chrono::system_clock::now();
		m_gen_hist.push_back((float)std::chrono::duration_cast<std::chrono::microseconds>(gen_end - gen_start).count());
		print_board(NULL);
	} // generation loop
	print_board("Final Board");
	_destroy_game();
}

void Game::_init_game() {
	// Create threads
	// Create game fields
	// Start the threads
	// Testing of your implementation will presume all threads are started here
	for (int i = 0; i < m_thread_num; ++i) {
		Thread* t = new GameThread(uint(i), &jobs);
		m_threadpool[i] = t;
	}
    current_field = new GameField(utils::read_lines(filename));
    next_field = new GameField(utils::read_lines(filename));
    m_thread_num = std::min(m_thread_num, (uint)current_field->field.size());
    for(Thread* t : m_threadpool){
        t->start();
    }
}

void Game::_step(uint curr_gen) {
	// Push jobs to queue
	// Wait for the workers to finish calculating 
	// Swap pointers between current and next field
    int height = current_field->field.size();
    int width = current_field->field[0].size();
    int job_size = height/m_thread_num;
    vector<vector<Lock>> locks(m_thread_num - 1, vector<Lock>(width, Lock()));
    int start = 0;
    int last = job_size - 1;
    for (int i = 0; i < m_thread_num; ++i){
        vector<Lock*> lower_p(width, NULL);
        vector<Lock*> upper_p(width, NULL);
        if(i != 0){ //if we are in the first job no need to lock...
            for (int k = 0; k < width; ++k) { //...lower_p (keep them NULL)
                lower_p[k] = &locks[i-1][k];
            }
        }
        if(i == m_thread_num - 1){ //if we are in the last job no need...
            last = height-1;       //...to lock upper_p (keep them NULL)
            //making sure the upper row of the last job is height-1
        }
        else{
            for (int k = 0; k < width; ++k) {
                upper_p[k] = &locks[i][k];
            }
        }
        Job j(uint(start), uint(last), current_field, next_field, lower_p, upper_p);
        jobs.push(j);
        start = last + 1;
        last += job_size;
    }
    for(Thread* t : m_threadpool){
        t->join();
    }
    GameField* temp = current_field;
    next_field = temp;
    current_field = next_field;
}

void Game::_destroy_game(){
	// Destroys board and frees all threads and resources 
	// Not implemented in the Game's destructor for testing purposes. 
	// Testing of your implementation will presume all threads are joined here
    delete current_field;
    delete next_field;
}

// TODO Implement
const vector<float> Game::gen_hist() const {
    vector<float> v;
    return v;
}

// TODO Implement
const vector<float> Game::tile_hist() const {
    vector<float> v;
    return v;
}

// TODO Implement
uint Game::thread_num() const {
    return 0;
}


