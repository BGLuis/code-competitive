import java.util.*
import java.math.BigInteger

class Cleaner(val h: Int, val l: Int, val u: Int, val m: Long)

class Rational(val num: Int, val den: Int) : Comparable<Rational> {
    override fun compareTo(other: Rational): Int {
        val p1 = num.toLong() * other.den
        val p2 = other.num.toLong() * den
        return p1.compareTo(p2)
    }

    override fun equals(other: Any?): Boolean {
        if (other !is Rational) return false
        return num.toLong() * other.den == other.num.toLong() * den
    }

    override fun hashCode(): Int {
        return 0
    }
}

class Event(val coord: Rational, val type: Int, val id: Int) : Comparable<Event> {
    override fun compareTo(other: Event): Int {
        val cmp = coord.compareTo(other.coord)
        if (cmp != 0) return cmp
        return type.compareTo(other.type)
    }
}

var bitCnt = LongArray(0)
var bitSum = LongArray(0)
var nh = 0
var distinctH = IntArray(0)

fun update(index: Int, valM: Long, valMH: Long) {
    var idx = index
    while (idx <= nh) {
        bitCnt[idx] += valM
        bitSum[idx] += valMH
        idx += idx and -idx
    }
}

fun getMinSumH(s: Long): Long {
    var idx = 0
    var currentCnt = 0L
    var currentSum = 0L
    var i = Integer.highestOneBit(nh)
    while (i > 0) {
        val nextIdx = idx + i
        if (nextIdx <= nh && currentCnt + bitCnt[nextIdx] < s) {
            idx = nextIdx
            currentCnt += bitCnt[idx]
            currentSum += bitSum[idx]
        }
        i = i shr 1
    }
    val remaining = s - currentCnt
    currentSum += remaining * distinctH[idx]
    return currentSum
}

fun gcd(a: Long, b: Long): Long = if (b == 0L) a else gcd(b, a % b)

fun compareFractions(n1: Long, d1: Long, n2: Long, d2: Long): Int {
    val p1 = BigInteger.valueOf(n1).multiply(BigInteger.valueOf(d2))
    val p2 = BigInteger.valueOf(n2).multiply(BigInteger.valueOf(d1))
    return p1.compareTo(p2)
}

fun main() {
    val sc = Scanner(System.`in`)
    if (!sc.hasNextInt()) return
    val n = sc.nextInt()
    val s = sc.nextLong()
    val k = sc.nextLong()

    val cleaners = mutableListOf<Cleaner>()
    val events = mutableListOf<Event>()
    val hSet = TreeSet<Int>()

    for (i in 0 until n) {
        val h = sc.nextInt()
        val l = sc.nextInt()
        val u = sc.nextInt()
        val m = k / h
        if (m >= 1) {
            val id = cleaners.size
            cleaners.add(Cleaner(h, l, u, m))
            events.add(Event(Rational(l, h), 0, id))
            events.add(Event(Rational(u, h), 1, id))
            hSet.add(h)
        }
    }

    if (cleaners.isEmpty()) {
        println("*")
        return
    }

    distinctH = hSet.toIntArray()
    nh = distinctH.size
    bitCnt = LongArray(nh + 1)
    bitSum = LongArray(nh + 1)

    Collections.sort(events)

    var bestNum = -1L
    var bestDen = 1L

    var i = 0
    while (i < events.size) {
        val currentCoord = events[i].coord
        var j = i
        while (j < events.size && events[j].coord == currentCoord) {
            if (events[j].type == 0) {
                val c = cleaners[events[j].id]
                val hIdx = Arrays.binarySearch(distinctH, c.h) + 1
                update(hIdx, c.m, c.m * c.h)
            }
            j++
        }

        var totalCnt = 0L
        var idx = nh
        while (idx > 0) {
            totalCnt += bitCnt[idx]
            idx -= idx and -idx
        }

        if (totalCnt >= s) {
            val sumH = getMinSumH(s)
            val curNum = currentCoord.num.toLong() * sumH
            val curDen = currentCoord.den.toLong()
            if (bestNum == -1L || compareFractions(curNum, curDen, bestNum, bestDen) < 0) {
                bestNum = curNum
                bestDen = curDen
            }
        }

        j = i
        while (j < events.size && events[j].coord == currentCoord) {
            if (events[j].type == 1) {
                val c = cleaners[events[j].id]
                val hIdx = Arrays.binarySearch(distinctH, c.h) + 1
                update(hIdx, -c.m, -c.m * c.h)
            }
            j++
        }
        i = j
    }

    if (bestNum == -1L) {
        println("*")
    } else {
        val common = gcd(bestNum, bestDen)
        println("${bestNum / common} ${bestDen / common}")
    }
}
