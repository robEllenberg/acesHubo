//  #include <rtt/Ports.hpp>
  #include <rtt/TaskContext.hpp>
  #include <rtt/Activity.hpp>
  using namespace RTT;
  using namespace std;

    class MyTask : public TaskContext {
        public:
            MyTask(string name) : TaskContext(name){
                //placeholder
            }
            bool configureHook(){
                //called on transition from preopertaion to stopped state
                //Return false to abort configuration.
                return true;
            }
            bool startHook(){
                //called on transition from stopped to running state
                //startup sequence is aborted by returning false
                return true;
            }
            void updateHook(){
                //called at each iteration of the component's loop
                printf("hello world\n");
            }
            void stopHook(){
                //called on transition from running to stopped states
            }
            void cleanupHook(){
                //called on transition from stopped to preoperation state
            }
    };

  int main(){
      MyTask* a_task = new MyTask("ATask");
      a_task->setActivity( new Activity( 5, 0.001 ));
      a_task->start();
      for(int i=0; i< 10000; i++){
          for(int j=0; j< 10000; j++);
      }
      a_task->stop();
      return 0;
  }
