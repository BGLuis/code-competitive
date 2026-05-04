import java.util.*
import java.math.BigInteger

var N_val = 0
var M_val = 0
var adj = Array(0) { mutableListOf<Int>() }
var sensorId = IntArray(0)
var failedMasks = Array(0) { mutableListOf<BigInteger>() }
var path_res = mutableListOf<Int>()

fun dfs(u: Int, forbiddenMask: BigInteger): Boolean {
    if (u == 2 * N_val) {
        path_res.add(u)
        return true
    }

    for (failed in failedMasks[u]) {
        if (forbiddenMask.and(failed) == failed) return false
    }

    for (v in adj[u]) {
        val s = sensorId[v]
        if (forbiddenMask.testBit(s)) continue

        if (dfs(v, forbiddenMask.setBit(s))) {
            path_res.add(u)
            return true
        }
    }

    failedMasks[u].add(forbiddenMask)
    return false
}

fun main() {
    val sc = Scanner(System.`in`)
    if (!sc.hasNextInt()) return
    N_val = sc.nextInt()
    M_val = sc.nextInt()

    adj = Array(2 * N_val + 1) { mutableListOf<Int>() }
    failedMasks = Array(2 * N_val + 1) { mutableListOf<BigInteger>() }
    sensorId = IntArray(2 * N_val + 1)

    for (i in 1..N_val) {
        sensorId[i] = i
        sensorId[i + N_val] = i
    }

    for (i in 0 until M_val) {
        val u = sc.nextInt()
        val v = sc.nextInt()
        adj[u].add(v)
    }

    val initialMask = BigInteger.ZERO.setBit(sensorId[1])
    if (dfs(1, initialMask)) {
        path_res.reverse()
        println(path_res.size)
        println(path_res.joinToString(" "))
    } else {
        println("*")
    }
}
