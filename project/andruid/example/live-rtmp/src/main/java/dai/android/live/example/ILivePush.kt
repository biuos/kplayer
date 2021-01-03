package dai.android.live.example

interface ILivePush<T> {

    fun startPush(url: String, any: T)

    fun stopPush()
}
