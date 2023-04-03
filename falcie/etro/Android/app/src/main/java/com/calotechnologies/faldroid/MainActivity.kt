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
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Favorite
import androidx.compose.material.icons.filled.MoreVert
import androidx.compose.material.icons.filled.PlayArrow
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.graphics.asImageBitmap
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.calotechnologies.faldroid.blockchain.Blockchain
import com.calotechnologies.faldroid.dataset.Dataset
//import com.calotechnologies.faldroid.blockchain.Blockchain
import com.calotechnologies.faldroid.dataset.cifar10.Cifar10Dataset
import com.calotechnologies.faldroid.dataset.iterator.DatasetIterator
import com.calotechnologies.faldroid.model.Model
import com.calotechnologies.faldroid.ui.theme.FaldroidTheme
import com.calotechnologies.faldroid.utils.bitmapFromFloatBuffer
import com.calotechnologies.faldroid.utils.directAllocateNativeFloatBuffer
import java.io.File
import java.nio.FloatBuffer
import java.nio.LongBuffer

//enum class

class MainActivity : ComponentActivity() {

    companion object {
        private const val EPOCH_1_50_SAVE_FILE = "onDeviceTrained50Epochs.ckpt"
        private const val EPOCH_1_100_SAVE_FILE = "onDeviceTrained50x2Epochs.ckpt"
        private const val EPOCH_1_150_SAVE_FILE = "onDeviceTrained50x3Epochs.ckpt"

        private const val EPOCH_100_50_SAVE_FILE = "100onDeviceTrained50Epochs.ckpt"
        private const val EPOCH_100_100_SAVE_FILE = "100onDeviceTrained50x2Epochs.ckpt"
        private const val EPOCH_100_150_SAVE_FILE = "100onDeviceTrained50x3Epochs.ckpt"

        ////////////////////////////////////////////////////////////////////////////////

        private const val FEDERATED_EPOCH_25_50_SAVE_FILE =
            "federatedOnDevice25BUTrained50Epochs.ckpt"
        private const val FEDERATED_EPOCH_25_100_SAVE_FILE =
            "federatedOnDevice25BUTrained50x2Epochs.ckpt"
        private const val FEDERATED_EPOCH_25_150_SAVE_FILE =
            "federatedOnDevice25BUTrained50x3Epochs.ckpt"

        private const val FEDERATED_EPOCH_50_50_SAVE_FILE =
            "federatedOnDevice50BUTrained50Epochs.ckpt"
        private const val FEDERATED_EPOCH_50_100_SAVE_FILE =
            "federatedOnDevice50BUTrained50x2Epochs.ckpt"
        private const val FEDERATED_EPOCH_50_150_SAVE_FILE =
            "federatedOnDevice50BUTrained50x3Epochs.ckpt"

        private const val FEDERATED_4_EPOCH_25_50_SAVE_FILE =
            "federated4OnDevice25BUTrained50Epochs.ckpt"
        private const val FEDERATED_4_EPOCH_25_100_SAVE_FILE =
            "federated4OnDevice25BUTrained50x2Epochs.ckpt"
        private const val FEDERATED_4_EPOCH_25_150_SAVE_FILE =
            "federated4OnDevice25BUTrained50x3Epochs.ckpt"

        private const val FEDERATED_4_EPOCH_50_50_SAVE_FILE =
            "federated4OnDevice50BUTrained50Epochs.ckpt"
        private const val FEDERATED_4_EPOCH_50_100_SAVE_FILE =
            "federated4OnDevice50BUTrained50x2Epochs.ckpt"
        private const val FEDERATED_4_EPOCH_50_150_SAVE_FILE =
            "federated4OnDevice50BUTrained50x3Epochs.ckpt"

        private const val FEDERATED_8_EPOCH_25_50_SAVE_FILE =
            "federated8OnDevice25BUTrained50Epochs.ckpt"
        private const val FEDERATED_8_EPOCH_25_100_SAVE_FILE =
            "federated8OnDevice25BUTrained50x2Epochs.ckpt"
        private const val FEDERATED_8_EPOCH_25_150_SAVE_FILE =
            "federated8OnDevice25BUTrained50x3Epochs.ckpt"

        private const val FEDERATED_8_EPOCH_50_50_SAVE_FILE =
            "federated8OnDevice50BUTrained50Epochs.ckpt"
        private const val FEDERATED_8_EPOCH_50_100_SAVE_FILE =
            "federated8OnDevice50BUTrained50x2Epochs.ckpt"
        private const val FEDERATED_8_EPOCH_50_150_SAVE_FILE =
            "federated8OnDevice50BUTrained50x3Epochs.ckpt"



        ////////////////////////////////////////////////////////////////////////////////

        private const val MODEL_1 = "cifar10_1.tflite"
        private const val MODEL_100 = "cifar10_100.tflite"

        private const val TAG = "MAIN_ACTIVITY_TAG:"
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val dataset = Cifar10Dataset(assets)
        dataset.dataPercent = 0.25f
        dataset.batchSize = 100
        dataset.normalize = true


        //val(loss, accuracy) = run(dataset, MODEL_1, EPOCH_1_100_SAVE_FILE, EPOCH_1_150_SAVE_FILE, 50, false)
        /*
        runFederated(
            dataset,
            2,
            MODEL_1,
            //FEDERATED_EPOCH_25_100_SAVE_FILE,
            //FEDERATED_EPOCH_25_150_SAVE_FILE,
            25,
            50,
            false
        )

         */


        val(loss, accuracy) = evaluate(Model(assets, MODEL_1), dataset, FEDERATED_8_EPOCH_50_150_SAVE_FILE)

        Log.i(TAG, "50 & 8 & $loss & ${accuracy * 100}")

        val (showTestImg, _) = dataset.getDatapoint(assets, 50037, false)
        val bitmap = bitmapFromFloatBuffer(showTestImg, 32, 32)




        setContent {
            FaldroidTheme {
                Surface(modifier = Modifier.fillMaxSize()) {
                    //MenuButtons()

                    Image(bitmap = bitmap.asImageBitmap(), contentDescription = null)

                    Conversation(
                        listOf(
                            //Message("$p1Guess == $p1Actual", ps1),
                            //Message(iotLoss, iotAccuracy),
                            Message("$loss", "$accuracy")
                        )
                    )
                }
            }
        }
    }

    private fun evaluate(
        model: Model,
        dataset: Dataset<FloatBuffer, LongBuffer>,
        weightsFilename: String?,
        ): Pair<Float, Float> {
        val loss = directAllocateNativeFloatBuffer(1)
        val accuracy = directAllocateNativeFloatBuffer(1)

        val (_, test_ds) = dataset.iterator()

        test_ds.batch(10000)

        if (weightsFilename != null)
            model.restore(filesDir, weightsFilename)

        Log.d(TAG, "Testing started")
        model.test(test_ds.next(), loss, accuracy)
        Log.d(TAG, "Testing ended")

        return Pair(loss[0], accuracy[0])
    }

    private fun runFederated(
        dataset: Dataset<FloatBuffer, LongBuffer>,
        numberOfModels: Int,
        modelFilename: String,
        restoreFilename: String?,
        saveFilename: String,
        blockUpdateEpochs: Int,
        trainingEpochs: Int = 50,
        evaluate: Boolean = false
    ): Pair<Float, Float> {

        val models = Array<Model>(numberOfModels) { Model(assets, modelFilename) }

        if (restoreFilename != null)
            models.forEach { it.restore(filesDir, restoreFilename) }

        val blockchain = Blockchain()
        val (train_ds, test_ds) = dataset.iterator()

        Log.d(TAG, "Training Started")

        Model.federatedTrain(
            filesDir,
            models,
            train_ds,
            blockchain,
            blockUpdateEpochs,
            trainingEpochs
        )

        models[0].save(filesDir, saveFilename)

        Log.d(TAG, "Training Finished")

        var finalLoss = 0.0f
        var finalAccuracy = 0.0f

        if (evaluate) {
            val e = evaluate(models[0], dataset, null)
            finalLoss = e.first
            finalAccuracy = e.second
        }

        return Pair(finalLoss, finalAccuracy)
    }

    private fun run(
        dataset: Dataset<FloatBuffer, LongBuffer>,
        modelFilename: String,
        restoreFilename: String?,
        saveFilename: String,
        trainingEpochs: Int = 50,
        evaluate: Boolean = false
    ): Pair<Float, Float> {
        val coreModel = Model(assets, modelFilename)

        val (training_batch, testing_batch) = dataset.iterator()

        Log.d(TAG, "Training Started")

        if (restoreFilename != null)
            coreModel.restore(filesDir, restoreFilename)

        coreModel.train(training_batch, trainingEpochs)
        coreModel.save(filesDir, saveFilename)

        Log.d(TAG, "Training Finished")
        var finalLoss = 0.0f
        var finalAccuracy = 0.0f

        if (evaluate) {
            val loss = directAllocateNativeFloatBuffer(1)
            val accuracy = directAllocateNativeFloatBuffer(1)

            Log.d(TAG, "Testing started")
            testing_batch.batch(10000)

            coreModel.test(testing_batch.next(), loss, accuracy)

            Log.d(TAG, "Testing ended")
            finalLoss = loss[0]
            finalAccuracy = accuracy[0]
        }

        return Pair(finalLoss, finalAccuracy)
    }

    private fun run(
        modelFilename: String,
        restoreFilename: String?,
        saveFilename: String?,
        trainingEpochs: Int = 50,
        dataPercent: Float = 0.25f,
        batchSize: Int = 100,
        normalize: Boolean = true,
        evaluate: Boolean = false
    ): Pair<Float, Float> {
        val coreModel = Model(assets, modelFilename)

        val dataset = Cifar10Dataset(assets)
        dataset.dataPercent = dataPercent
        dataset.batchSize = batchSize
        dataset.normalize = normalize

        val (training_batch, testing_batch) = dataset.iterator()

        Log.d(TAG, "Training Started")

        if (restoreFilename != null)
            coreModel.restore(filesDir, restoreFilename)

        coreModel.train(training_batch, trainingEpochs)

        if (saveFilename != null)
            coreModel.save(filesDir, saveFilename)

        Log.d(TAG, "Training Finished")
        var finalLoss = 0.0f
        var finalAccuracy = 0.0f

        if (evaluate) {
            val loss = directAllocateNativeFloatBuffer(1)
            val accuracy = directAllocateNativeFloatBuffer(1)

            Log.d(TAG, "Testing started")
            testing_batch.batch(10000)

            coreModel.test(testing_batch.next(), loss, accuracy)

            Log.d(TAG, "Testing ended")
            finalLoss = loss[0]
            finalAccuracy = accuracy[0]
        }

        return Pair(finalLoss, finalAccuracy)
    }
}

@Composable
private fun MainScreen() {

}

@Preview(
    uiMode = Configuration.UI_MODE_NIGHT_YES,
    showBackground = true
)
@Composable
private fun MenuButtons() {
    //var expanded by remember { mutableStateOf(false) }
    Column(modifier = Modifier.padding(16.dp)) {
        Text(
            text = "Training Parameters",
            modifier = Modifier.padding(bottom = 8.dp),
            style = MaterialTheme.typography.bodyMedium
        )

        var modelFilename by remember { mutableStateOf("") }//: String,
        var restoreFilename by remember { mutableStateOf(null) }//: String?,
        var saveFilename by remember { mutableStateOf(null) }//: String?,
        var trainingEpochs by remember { mutableStateOf(50) }//: Int = 50,
        var dataPercent by remember { mutableStateOf(0.25f) }//: Float = 0.25f,
        var batchSize by remember { mutableStateOf(100) }//: Int = 100,
        var normalize by remember { mutableStateOf(true) }//: Boolean = true,
        var evaluate by remember { mutableStateOf(false) }//: Boolean = false


        OutlinedTextField(
            value = "Model Filename",
            onValueChange = { },
            label = { Text("Name") }
        )

        Spacer(modifier = Modifier.height(4.dp))

        //ParameterButton(text = "Model") {

    }
}
/*
Box(
    modifier = Modifier
        .fillMaxSize()
        .wrapContentSize(Alignment.TopEnd)
) {
    IconButton(onClick = { expanded = true }) {
        Icon(Icons.Default.MoreVert, contentDescription = "Localized description")
    }
    DropdownMenu(
        expanded = expanded,
        onDismissRequest = { expanded = false }
    ) {
        DropdownMenuItem(onClick = { /* Handle refresh! */ }, text = {
            Text("Refresh")
        })
        DropdownMenuItem(onClick = { /* Handle settings! */ }, text = {
            Text("Settings")
        })
        Divider()
        DropdownMenuItem(onClick = { /* Handle send feedback! */ }, text = {
            Text("Send Feedback")
        })
    }
}

 */

/*
@Composable
private fun ParameterButton(text: String, onClick: () -> Unit) {
    var expanded by remember { mutableStateOf(false) }

    Button(onClick = onClick) {
        Text(text)
    }
    DropdownMenu(
        expanded = expanded,
        onDismissRequest = { expanded = false }
    ) {
        DropdownMenuItem(onClick = { /* Handle refresh! */ }) {
            Text("Refresh")
        }
        DropdownMenuItem(onClick = { /* Handle settings! */ }) {
            Text("Settings")
        }
        Divider()
        DropdownMenuItem(onClick = { /* Handle send feedback! */ }) {
            Text("Send Feedback")
        }
    }
}

 */

@Composable
private fun RunTrainingButton() {

    Button(onClick = { }) {
        Icon(
            Icons.Filled.PlayArrow,
            contentDescription = null,
            modifier = Modifier.size(ButtonDefaults.IconSize)
        )
        Spacer(Modifier.size(ButtonDefaults.IconSpacing))
        Text("Run")
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

/*
@Preview(name = "Light Mode")
@Preview(
    uiMode = Configuration.UI_MODE_NIGHT_YES,
    showBackground = true,
    name = "Dark Mode"
)

 */
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

