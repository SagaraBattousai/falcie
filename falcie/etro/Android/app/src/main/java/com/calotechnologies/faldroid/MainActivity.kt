package com.calotechnologies.faldroid

import android.content.res.Configuration
import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.animation.animateColorAsState
import androidx.compose.animation.animateContentSize
import androidx.compose.foundation.Image
import androidx.compose.foundation.border
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.graphics.asImageBitmap
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.calotechnologies.faldroid.blockchain.Blockchain
import com.calotechnologies.faldroid.buffer.FloatDataBuffer
//import com.calotechnologies.faldroid.blockchain.Blockchain
import com.calotechnologies.faldroid.dataset.cifar10.Cifar10Classes
import com.calotechnologies.faldroid.dataset.cifar10.Cifar10Dataset
import com.calotechnologies.faldroid.model.Model
import com.calotechnologies.faldroid.ui.theme.FaldroidTheme
import com.calotechnologies.faldroid.utils.bitmapFromFloatBuffer
import com.calotechnologies.faldroid.utils.directAllocateNativeFloatBuffer
import com.calotechnologies.faldroid.utils.loadModelFile
import org.tensorflow.lite.Interpreter
import java.io.File
import java.nio.FloatBuffer

class MainActivity : ComponentActivity() {

    companion object {
        private const val EPOCH_50_SAVE_FILE = "onDeviceTrained50Epochs.ckpt"
        private const val EPOCH_100_SAVE_FILE = "onDeviceTrained50x2Epochs.ckpt"
        private const val EPOCH_150_SAVE_FILE = "onDeviceTrained50x3Epochs.ckpt"

        private const val TAG = "MAIN_ACTIVITY_TAG:"
    }
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        /*
        val model = Model(assets, "cifar10.tflite")
        //val model100 = Model(assets, "cifar10_100.tflite")

        val dataset = Cifar10Dataset(assets)
        dataset.dataPercent = 0.25f
        dataset.batchSize = 100
        dataset.normalize = true

        //val (img, label) = dataset.getTrainingData(assets, 20, true)

        val (testImg, testLabel) = dataset.getDatapoint(assets, 50037, true)
        val (showTestImg, _) = dataset.getDatapoint(assets, 50037, false)

        val probs1: FloatBuffer = model.infer(testImg)["output"] as FloatBuffer
        testImg.rewind() // needed? TODO:test
        var ps1: String = ""
        for (x in 0 until 10)
            ps1 += "${probs1[x]}\n"
        val p1Guess = Cifar10Classes.mapProbabilityToClass(probs1)
        val p1Actual = Cifar10Classes.labelToClass(testLabel[0])

        //val iter = dataset.DataBatchIterator(100, 0.25f, true)

        //model.restore(filesDir, fname)
        //model.train(dataset.datasetIterator(), 50)
        //model.save(filesDir, fname)

        //Why does this one VV work and that one ^^ not
        //model.train(img, label, 10000 / 100, 100)

        val probs2: FloatBuffer = model.infer(testImg)["output"] as FloatBuffer
        testImg.rewind()
        var ps2: String = ""
        for (x in 0 until 10)
            ps2 += "${probs2[x]}\n"
        val p2Guess = Cifar10Classes.mapProbabilityToClass(probs2)
        val p2Actual = Cifar10Classes.labelToClass(testLabel[0])

        val bitmap = bitmapFromFloatBuffer(showTestImg, 32, 32)
        */

        val blockchain = Blockchain()
        val weights = arrayOf(
            arrayOf(directAllocateNativeFloatBuffer(2), directAllocateNativeFloatBuffer(1)),
            arrayOf(directAllocateNativeFloatBuffer(2), directAllocateNativeFloatBuffer(1))
        )

        //Also testing if required to rewind  :)
        weights[0][0].put(1.0f)
        weights[0][0].put(2.0f)

        //weights[0][0].rewind()

        weights[0][1].put(3.0f)
        //weights[0][1].rewind()

        weights[1][0].put(4.0f)
        weights[1][0].put(5.0f)

        //weights[1][0].rewind()

        weights[1][1].put(6.0f)
        //weights[1][1].rewind()




        val federatedWeights = arrayOf(
            directAllocateNativeFloatBuffer(2), directAllocateNativeFloatBuffer(1)
        )

        //val out =
        blockchain.federate(weights, longArrayOf(8L, 8L), federatedWeights)

        Log.d(TAG, "pause here")

        setContent {
            FaldroidTheme {
                Surface(modifier = Modifier.fillMaxSize()) {

                    //Image(bitmap = bitmap.asImageBitmap(), contentDescription = null)

                    Conversation(
                        listOf(
                            Message("weights[0][0].0", "${weights[0][0].get(0)}"),
                            Message("weights[0][0].1", "${weights[0][0].get(1)}"),
                            Message("weights[0][1].0", "${weights[0][1].get(0)}"),

                            Message("weights[1][0].0", "${weights[1][0].get(0)}"),
                            Message("weights[1][0].1", "${weights[1][0].get(1)}"),
                            Message("weights[1][1].0", "${weights[1][1].get(0)}"),

                            Message("federatedWeights[0].0", "${federatedWeights[0].get(0)}"),
                            Message("federatedWeights[0].1", "${federatedWeights[0].get(1)}"),
                            Message("federatedWeights[1].0", "${federatedWeights[1].get(0)}")
                            //Message("$p1Guess == $p1Actual", ps1),
                            //Message("$p2Guess == $p2Actual", ps2)
                        )
                    )


                }
            }
        }
    }
}

data class Message(val author: String, val body: String)

@Composable
fun MessageCard(msg: Message) {
    Row(modifier = Modifier.padding(all = 8.dp)) {
        Image(
            painter = painterResource(R.drawable.truck),
            contentDescription = "Inference Image",
            modifier = Modifier
                .size(40.dp)
                .clip(CircleShape)
                .border(1.5.dp, MaterialTheme.colorScheme.secondary, CircleShape)
        )
        Spacer(modifier = Modifier.width(8.dp))

        var isExpanded by remember { mutableStateOf(false) }
        val surfaceColor by animateColorAsState(
            if (isExpanded) MaterialTheme.colorScheme.primary else MaterialTheme.colorScheme.surface
        )

        Column(modifier = Modifier.clickable { isExpanded = !isExpanded }) {
            Text(
                text = msg.author,
                color = MaterialTheme.colorScheme.primary,
                style = MaterialTheme.typography.titleMedium
            )
            Spacer(modifier = Modifier.height(4.dp))

            Surface(
                shape = MaterialTheme.shapes.medium,
                tonalElevation = 1.dp,
                color = surfaceColor,
                modifier = Modifier
                    .animateContentSize()
                    .padding(1.dp)
            ) {
                Text(
                    text = msg.body,
                    modifier = Modifier.padding(all = 4.dp),
                    maxLines = if (isExpanded) Int.MAX_VALUE else 1,
                    style = MaterialTheme.typography.bodyMedium
                )
            }
        }
    }
}

@Composable
fun Conversation(messages: List<Message>) {
    LazyColumn {
        items(messages) { message ->
            MessageCard(msg = message)
        }
    }
}

@Preview(name = "Light Mode")
@Preview(
    uiMode = Configuration.UI_MODE_NIGHT_YES,
    showBackground = true,
    name = "Dark Mode"
)
@Composable
fun PreviewConversation() {
    FaldroidTheme {
        Conversation(
            messages = listOf(
                Message("getWeights", "W1"),
                Message("setWeights", "W2"),
                Message("getWeights", "W3")
            )
        )

    }
}
