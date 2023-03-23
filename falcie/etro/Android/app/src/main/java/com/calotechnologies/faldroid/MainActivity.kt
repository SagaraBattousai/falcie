package com.calotechnologies.faldroid

import android.content.res.Configuration
import android.os.Bundle
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
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.calotechnologies.faldroid.ui.theme.FaldroidTheme
import com.calotechnologies.faldroid.utils.loadModelFile
import org.tensorflow.lite.Interpreter
import java.io.File
import java.nio.FloatBuffer

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        ///////////////////////////
        val model = Interpreter(loadModelFile(applicationContext.assets, "25_cifar10.tflite"))

        val inputs: MutableMap<String, Any> = HashMap()
        inputs["unused"] = ""
        //inputs["x"] = trainImageBatches[0];
        //inputs["y"] = trainLabelBatches[0];
        //val inputs: Array<Any> = arrayOf(trainImageBatches[0])//, trainLabelBatches[0])

        val outputs: MutableMap<String, Any> = HashMap()
        val layer_0 = FloatBuffer.allocate(3*3*3*32)
        val layer_1 = FloatBuffer.allocate(32)
        val layer_2 = FloatBuffer.allocate(3*3*32*64)
        val layer_3 = FloatBuffer.allocate(64)
        val layer_4 = FloatBuffer.allocate(3*3*64*64)
        val layer_5 = FloatBuffer.allocate(64)
        val layer_6 = FloatBuffer.allocate(1024 * 64)
        val layer_7 = FloatBuffer.allocate(64)
        val layer_8 = FloatBuffer.allocate(64 * 10)
        val layer_9 = FloatBuffer.allocate(10)

        outputs["layer_0"] = layer_0
        outputs["layer_1"] = layer_1
        outputs["layer_2"] = layer_2
        outputs["layer_3"] = layer_3
        outputs["layer_4"] = layer_4
        outputs["layer_5"] = layer_5
        outputs["layer_6"] = layer_6
        outputs["layer_7"] = layer_7
        outputs["layer_8"] = layer_8
        outputs["layer_9"] = layer_9


        model.runSignature(inputs, outputs, "getWeights");

        val msg = Message("first", "${layer_0.get(0)}")
        ///////////////////////////

        layer_0.rewind()
        layer_0.put(5.0f)
        layer_0.rewind()

        val inputs2: MutableMap<String, Any> = HashMap()

        inputs2["l1"] = layer_0
        inputs2["l2"] = layer_1
        inputs2["l3"] = layer_2
        inputs2["l4"] = layer_3
        inputs2["l5"] = layer_4
        inputs2["l6"] = layer_5
        inputs2["l7"] = layer_6
        inputs2["l8"] = layer_7
        inputs2["l9"] = layer_8
        inputs2["l10"] = layer_9

        val outputs2: MutableMap<String, Any> = HashMap()

        val outputFile = File(filesDir, "temp_blockchainWeights.ckpt")
        inputs2["checkpoint_path"] = outputFile.absolutePath

        model.runSignature(inputs2, outputs2, "saveTmp")

        val inputs3: MutableMap<String, Any> = HashMap()
        inputs3["checkpoint_path"] = outputFile.absolutePath

        val outputs3: MutableMap<String, Any> = HashMap()

        model.runSignature(inputs3, outputs3, "restore")
        ///////////////////////////

        val inputs4: MutableMap<String, Any> = HashMap()
        inputs4["unused"] = ""
        //inputs["x"] = trainImageBatches[0];
        //inputs["y"] = trainLabelBatches[0];
        //val inputs: Array<Any> = arrayOf(trainImageBatches[0])//, trainLabelBatches[0])

        val outputs4: MutableMap<String, Any> = HashMap()

        /*
        val layer_0 = FloatBuffer.allocate(3*3*3*32)
        val layer_1 = FloatBuffer.allocate(32)
        val layer_2 = FloatBuffer.allocate(3*3*32*64)
        val layer_3 = FloatBuffer.allocate(64)
        val layer_4 = FloatBuffer.allocate(3*3*64*64)
        val layer_5 = FloatBuffer.allocate(64)
        val layer_6 = FloatBuffer.allocate(1024 * 64)
        val layer_7 = FloatBuffer.allocate(64)
        val layer_8 = FloatBuffer.allocate(64 * 10)
        val layer_9 = FloatBuffer.allocate(10)

         */

        outputs4["layer_0"] = layer_0.rewind()
        outputs4["layer_1"] = layer_1.rewind()
        outputs4["layer_2"] = layer_2.rewind()
        outputs4["layer_3"] = layer_3.rewind()
        outputs4["layer_4"] = layer_4.rewind()
        outputs4["layer_5"] = layer_5.rewind()
        outputs4["layer_6"] = layer_6.rewind()
        outputs4["layer_7"] = layer_7.rewind()
        outputs4["layer_8"] = layer_8.rewind()
        outputs4["layer_9"] = layer_9.rewind()


        model.runSignature(inputs, outputs, "getWeights");

        val msg2 = Message("first", "${layer_0.get(0)}")



        /////////////////////////////////


        val messages: List<Message> = listOf(msg, msg2)//Message("tmp", "pmt"))
        setContent {
            FaldroidTheme {
                Surface(modifier = Modifier.fillMaxSize()) {
                    //MessageCard(Message("James", "True Msg"))
                    Conversation(messages = messages)
                }
            }
        }
    }
}

data class Message(val author: String, val body: String)

@Composable
fun MessageCard(msg: Message) {
    Row (modifier = Modifier.padding(all=8.dp)) {
        Image(
            painter = painterResource(R.drawable.truck),
            contentDescription = "Inference Image",
            modifier = Modifier
                .size(40.dp)
                .clip(CircleShape)
                .border(1.5.dp, MaterialTheme.colorScheme.secondary, CircleShape)
        )
        Spacer(modifier=Modifier.width(8.dp))

        var isExpanded by remember { mutableStateOf(false)}
        val surfaceColor by animateColorAsState(
            if (isExpanded) MaterialTheme.colorScheme.primary else MaterialTheme.colorScheme.surface
        )

        Column (modifier = Modifier.clickable { isExpanded = !isExpanded }) {
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
        Conversation(messages = listOf(
            Message("getWeights", "W1"),
            Message("setWeights", "W2"),
            Message("getWeights", "W3")
            ))

    }
}
