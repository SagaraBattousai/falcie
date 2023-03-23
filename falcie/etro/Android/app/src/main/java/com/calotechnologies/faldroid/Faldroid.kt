/*
package com.calotechnologies.faldroid

import android.content.Context
import android.graphics.Bitmap
import android.os.SystemClock
import android.util.Log
import com.calotechnologies.faldroid.utils.loadModelFile
import com.google.android.gms.tflite.client.TfLiteInitializationOptions
import com.google.android.gms.tflite.gpu.support.TfLiteGpu
import org.tensorflow.lite.DataType
import org.tensorflow.lite.InterpreterApi
import org.tensorflow.lite.support.tensorbuffer.TensorBuffer
import org.tensorflow.lite.task.core.BaseOptions
import org.tensorflow.lite.task.gms.vision.TfLiteVision
import java.nio.ByteBuffer

//import org.tensorflow.lite.Interpreter;

class Faldroid(
    private val context: Context,
    private val listener: ModelListener,
    var numThreads: Int = 2,
    var currentDelegate: Int = 0
) {
    private val TAG = "Faldroid"

    // For this example this needs to be a var so it can be reset on changes. If the ObjectDetector
    // will not change, a lazy val would be preferable.
    //private var objectDetector: ObjectDetector? = null
    private var gpuSupported = false

    //private val model: AutoModel25Cifar10 = AutoModel25Cifar10.newInstance(context)
    init {

        TfLiteGpu.isGpuDelegateAvailable(context).onSuccessTask { gpuAvailable: Boolean ->
            val optionsBuilder =
                TfLiteInitializationOptions.builder()
            if (gpuAvailable) {
                optionsBuilder.setEnableGpuDelegateSupport(true)
            }
            TfLiteVision.initialize(context, optionsBuilder.build())
        }.addOnSuccessListener {
            listener.onInitialized()
        }.addOnFailureListener {
            listener.onError(
                "TfLiteVision failed to initialize: "
                        + it.message
            )
        }
    }

    //fun clearObjectDetector() {
    //model = null
    //}

    // Initialize the object detector using current settings on the
    // thread that is using it. CPU and NNAPI delegates can be used with detectors
    // that are created on the main thread and used on a background thread, but
    // the GPU delegate needs to be used on the thread that initialized the detector
    fun setupObjectDetector() {
        if (!TfLiteVision.isInitialized()) {
            Log.e(TAG, "setupObjectDetector: TfLiteVision is not initialized yet")
            return
        }

        // Set general detection options, including number of used threads
        val baseOptionsBuilder = BaseOptions.builder().setNumThreads(numThreads)

        // Use the specified hardware for running the model. Default to CPU
        when (currentDelegate) {
            DELEGATE_CPU -> {
                // Default
            }
            DELEGATE_GPU -> {
                if (gpuSupported) {
                    baseOptionsBuilder.useGpu()
                } else {
                    listener.onError("GPU is not supported on this device")
                }
            }
            DELEGATE_NNAPI -> {
                baseOptionsBuilder.useNnapi()
            }
        }
    }

        fun detect(byteBuffer: ByteBuffer) { //image: Bitmap) {
            if (!TfLiteVision.isInitialized()) {
                Log.e(TAG, "detect: TfLiteVision is not initialized yet")
                return
            }

            //if (objectDetector == null) {
            //setupObjectDetector() //TODO: ??????????????
            //}

            // Inference time is the difference between the system time at the start and finish of the
            // process
            var inferenceTime = SystemClock.uptimeMillis()

            // Create preprocessor for the image.
            // See https://www.tensorflow.org/lite/inference_with_metadata/
            //            lite_support#imageprocessor_architecture
            //val imageProcessor = ImageProcessor.Builder().add(Rot90Op(-imageRotation / 90)).build()


            // Creates inputs for reference.
            val inputFeature0 =
                TensorBuffer.createFixedSize(intArrayOf(1, 32, 32, 3), DataType.FLOAT32)
            inputFeature0.loadBuffer(byteBuffer)

// Runs model inference and gets result.
            //val outputs = model.process(inputFeature0)


            // Preprocess the image and convert it into a TensorImage for detection.
            //val tensorImage = imageProcessor.process(TensorImage.fromBitmap(image)

            //val results = objectDetector?.detect(tensorImage)
            inferenceTime = SystemClock.uptimeMillis() - inferenceTime
            listener.onResults(
                //outputs,
                inferenceTime,
            )
        }

        interface ModelListener {
            fun onInitialized()
            fun onError(error: String)
            fun onResults(
                //results: AutoModel25Cifar10.Outputs,
                inferenceTime: Long

                //results: MutableList<Detection>?,
                //inferenceTime: Long,
                //imageHeight: Int,
                //imageWidth: Int
            )
        }

        companion object {
            const val DELEGATE_CPU = 0
            const val DELEGATE_GPU = 1
            const val DELEGATE_NNAPI = 2
            const val MODEL_MOBILENETV1 = 0
            const val MODEL_EFFICIENTDETV0 = 1
            const val MODEL_EFFICIENTDETV1 = 2
            const val MODEL_EFFICIENTDETV2 = 3
        }
    }

 */


/*
{
    companion object
    {
        private const val TAG: String = "FaldroidClient"
        private const val modelFilename = "25_cifar10.tflite"
    }

    //private lateinit var context: Context
    private var tflite: Interpreter?

    private loadModel(): unit
    {
        try {
            val buffer = loadModelFile(context.assets, modelFilename)

        }
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
package com.calotechnologies.faldroid

import android.graphics.BitmapFactory
import android.os.Bundle
import android.view.Menu
import android.view.MenuItem
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.WindowCompat
import androidx.navigation.ui.AppBarConfiguration
import com.calotechnologies.faldroid.databinding.ActivityMainBinding
import com.calotechnologies.faldroid.utils.loadImage
import com.calotechnologies.faldroid.utils.loadModelFile

import org.tensorflow.lite.Interpreter
import java.nio.FloatBuffer


class MainActivity : AppCompatActivity() {

    private lateinit var appBarConfiguration: AppBarConfiguration
    private lateinit var binding: ActivityMainBinding
    private lateinit var model: Interpreter //Api

    //val initializeTask: Task<Void> by lazy { TfLite.initialize(this) }

    override fun onCreate(savedInstanceState: Bundle?) {
        WindowCompat.setDecorFitsSystemWindows(window, false)
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        /*
        lifecycleScope.launchWhenStarted { // uses coroutine
            Tasks.await(initializeTask)//.//await()
        }

         */

        var s: String = ""

        //initializeTask.addOnSuccessListener {
        //var interpreterOptions = InterpreterApi.Options().setRuntime(InterpreterApi.Options.TfLiteRuntime.FROM_SYSTEM_ONLY)
        model = Interpreter(loadModelFile(applicationContext.assets, "25_cifar10.tflite"))
        //InterpreterApi.create(loadModelFile(applicationContext.assets,
        //    "25_cifar10.tflite"), interpreterOptions)

        //Interpreter()



        //int NUM_EPOCHS = 100;
        //int BATCH_SIZE = 100;
        val IMG_WIDTH: Int = 32;
        val IMG_HEIGHT: Int = 32;
        val IMG_CHANNELS: Int = 3;
        //int NUM_TRAININGS = 60000;
        //int NUM_BATCHES = NUM_TRAININGS / BATCH_SIZE;

        val trainImageBatches: MutableList<FloatBuffer> = ArrayList(1)
        val trainLabelBatches: MutableList<FloatBuffer> = ArrayList(1);

        // Prepare training batches.
        val trainImages: FloatBuffer =
            loadImage(applicationContext.assets, "2.png", IMG_WIDTH, IMG_HEIGHT)

        val trainLabels: FloatBuffer = FloatBuffer.allocate(1)



        trainLabels.put(6.0f)
        trainImages.rewind()
        trainLabels.rewind()

        // Fill the data values...
        trainImageBatches.add(trainImages)
        trainLabelBatches.add(trainLabels)

        // Run training for a few steps.
        //float[] losses = new float[NUM_EPOCHS];
        //for (int epoch = 0; epoch < NUM_EPOCHS; ++epoch) {
            //for (int batchIdx = 0; batchIdx < NUM_BATCHES; ++batchIdx) {

            val inputs: MutableMap<String, Any> = HashMap();
            inputs["x"] = trainImageBatches[0];
            //inputs["y"] = trainLabelBatches[0];
            //val inputs: Array<Any> = arrayOf(trainImageBatches[0])//, trainLabelBatches[0])

            val outputs: MutableMap<String, Any> = HashMap();
            //val outputs: MutableMap<Int, Any> = HashMap();
            val probs: FloatBuffer = FloatBuffer.allocate(10);
            val logits: FloatBuffer = FloatBuffer.allocate(10);
        outputs["output"] = probs
        //outputs[0] = probs
        //outputs[1] = logits
        outputs["logits"] = logits

        model.runSignature(inputs, outputs, "infer");
        //model.r  //.//runForMultipleInputsOutputs(inputs, outputs);


        //var s: String = ""
        for (i in 0..9 )
        {
            s += "${probs.get(i)}\n"
        }
       // }
        //    .addOnFailureListener { e ->
        //        Log.e("Interpreter", "Cannot initialize interpreter", e)
        //    }

        binding.button.setOnClickListener { view ->
            binding.textView.text = s
            //Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
            //    .setAnchorView(R.id.fab)
            //    .setAction("Action", null).show()
        }

        binding.imageView.setImageBitmap(BitmapFactory.decodeStream(
                applicationContext.assets.open("2.png"))
        )
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        // Inflate the menu; this adds items to the action bar if it is present.
        menuInflater.inflate(R.menu.menu_main, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        return when (item.itemId) {
            R.id.action_settings -> true
            else -> super.onOptionsItemSelected(item)
        }
    }

    /*
    override fun onSupportNavigateUp(): Boolean {
        val navController = findNavController(R.id.nav_host_fragment_content_main)
        return navController.navigateUp(appBarConfiguration)
                || super.onSupportNavigateUp()
    }
    */
}
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
 */
