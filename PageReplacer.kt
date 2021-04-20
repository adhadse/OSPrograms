package src

class PageReplacer(
    private var requests: MutableList<Int>,
    val _totalframes: Int
) {
    private var nextToRemove = mutableListOf<Int>()
    private var missOrHit = mutableListOf<String>()
    private var frames = arrayListOf<Int>()
    private val totalframes = _totalframes

    private fun replacer(request:Int) {
        print("Not found ${request} Replacing ${frames.indexOf(nextToRemove.first())} | ${frames}")
        frames[frames.indexOf(nextToRemove.first())] = request
        nextToRemove.removeFirst()
        nextToRemove.add(request)
    }

    fun execute() {
        for (request in requests) {
            if (frames.size != totalframes) {
                if (frames.contains(request)) {
                    missOrHit.add("H")
                    continue
                }
                missOrHit.add("M")
                frames.add(request)
                nextToRemove.add(request)
                continue
            }
             else {
                if (frames.contains(request)) {
                    missOrHit.add("H")
                    continue
                }
                missOrHit.add("M")
                replacer(request)
            }
        }
    }

    fun showResult() {
        var miss:Float = 0f
        var hit:Float = 0f
        for (result in missOrHit) {
            when (result) {
                "H" -> hit += 1
                "M" -> miss += 1
            }
        }
        val missRatio:Float = (miss/requests.size).toFloat()
        val hitRatio:Float = (hit/requests.size).toFloat()
        print("Total Frames ${totalframes}")
        print("Total no of Miss ${miss}\nTotal no of Hits ${hit}")
        print("Miss Ratio: ${missRatio}\nHit Ratio ${hitRatio}")
    }
}

fun main() {
    val request = mutableListOf(1,2,3,4,1,2,5,1,2,3,4,5)
    val defaultreplacer = PageReplacer(request, 4)
    defaultreplacer.execute()
    defaultreplacer.showResult()
}