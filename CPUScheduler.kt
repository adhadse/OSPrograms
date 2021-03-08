/**
 *  @author Anurag Dhadse
 *  Below code is written in Kotlin (JVM)
 */
package src

class Process(
    val processId: Int,
    val arrivalTime: Int,
    val burstTime: Int,
    var completionTime: Int? = null,
    var turnAroundTime: Int? = null,
    var waitingTime: Int? = null,
    var completed: Boolean = false,
) {
    fun setTurnAroundTime() {
        turnAroundTime = completionTime?.minus(arrivalTime)
    }
    
    fun setWaitingTime() {
        waitingTime = turnAroundTime?.minus(burstTime)
    }
}

open class Scheduler(
    private var clock:Int = 0,
    private var processList: MutableList<Process>
) {
    fun arrivedProcesses(): MutableList<Process>? {
        // This function creates a list of all process which are not completed
        // and has arrived after CPU clock finished executing process.
        val incompleteProcesses =  processList.filter { !it.completed }
        return if (incompleteProcesses.isNotEmpty()) {
            val arrivedProcesses = mutableListOf<Process>()
            for (process in incompleteProcesses) {
                if (process.arrivalTime <= clock){
                    // If process has already arrived add first of those process
                    arrivedProcesses.add(process)
                } else if (arrivedProcesses.isNullOrEmpty()) {
                    // When Clock is IDLE, no process has arrived
                    incompleteProcesses.minByOrNull { it.arrivalTime }?.let { arrivedProcesses.add(it) }
                }
            }
    //            print("incomplete processes $incompleteProcesses")
    //            print("first arrival time $firstArrivedTime")
    //            print("first arrived process $firstArrivedProcesses")
            arrivedProcesses
        } else {
            null
        }
    }
    open fun nextProcess(): Process? {
        // This function finds the next suitable process
        // Override in child class
        // since execute() calls it repeatedly and stops once this function return null
        print("Using Scheduler Class (Default)")
        return null
    }
    open fun execute() {
        // This function execute the process one by one by calling nextProcess()
        while (processList.any { it.completed == false }) {

            // First Next Suitable process to execute
            val process = nextProcess() ?: return
            println("ProcessID ${process.processId}\n-------------------------")

            // Set the clock to arrival of late arrived process arrival time i.e. CPU was sitting idle.
            if (clock < process.arrivalTime) { clock = process.arrivalTime }
            clock += process.burstTime
            process.completionTime = clock
            process.setTurnAroundTime()
            process.setWaitingTime()
            process.completed = true
        }
    }
    fun showSchedule() {
        // Prints the scheduling time and showing the times of process
        print("Process_id Arrival_Time Burst_Time Completion_Time Turn_Around_Time Waiting_Time")
        var avgTurnAroundTime = 0f
        var avgWaitingTime = 0f
        for (process in processList) {
            print("${process.processId}" +
                    "          ${process.arrivalTime}" +
                    "            ${process.burstTime}" +
                    "          ${process.completionTime}" +
                    "               ${process.turnAroundTime}" +
                    "                ${process.waitingTime}")
            avgTurnAroundTime += process.turnAroundTime!!
            avgWaitingTime += process.waitingTime!!
        }
        print("Average Turn Around Time ${avgTurnAroundTime/processList.size}")
        print("Average Waiting Time ${avgWaitingTime/processList.size}")
    }
}

// Inherits From Scheduler Class
class FirstComeFirstServe(
    private var clock:Int = 0,
    private var processList: MutableList<Process>
): Scheduler(clock, processList) {

    override fun nextProcess(): Process? {
        return arrivedProcesses()?.minByOrNull{it.arrivalTime}
    }
}

// Inherits From Scheduler Class
class SJF(
    private var clock:Int = 0,
    private var processList: MutableList<Process>
): Scheduler(clock, processList) {
    override fun nextProcess(): Process? {
        // Finds next suitable process by first finding
        // all first arrived process,not completed
        // Then in those process finding least burst time process
        return arrivedProcesses()?.minByOrNull { it.burstTime }
    }
}

fun main() {
   sjf()
}

fun fcfs() {
    val processlist = mutableListOf(
        Process(1, 3, 4),
        Process(2, 5, 3),
        Process(3, 0, 2),
        Process(4, 5, 1),
        Process(5, 4, 3)
    )
    val fcfs = FirstComeFirstServe(clock=0, processList = processlist )
    fcfs.execute()
    fcfs.showSchedule()
}

fun sjf() {
    val processlist = mutableListOf(
        Process(1, 2, 6),
        Process(2, 5, 2),
        Process(3, 1, 8),
        Process(4, 0, 3),
        Process(5, 4, 4)
    )
    val sjf = SJF(clock=0, processList = processlist )
    sjf.execute()
    sjf.showSchedule()
}

