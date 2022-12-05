using System.Collections;
using System.Collections.Generic;
using System.IO.Ports;
using UnityEngine;

public class MoveController : MonoBehaviour
{
    public Transform aStick;
    public Renderer button0, button1, button2, button3, button4, button5, stickButton;

    Color defaultColor;

    SerialPort serial;

    float x, y, z;
    
    void Start()
    {
        serial = new SerialPort(PlayerPrefs.GetString("portSelected"), 115200);

        defaultColor = button0.material.color;
        serial.Open();
        serial.ReadTimeout = 2000;
    }

    void Update()
    {
        /* Gyro */

        serial.Write("a");
        float ax = int.Parse(serial.ReadLine());
        serial.Write("b");
        float ay = int.Parse(serial.ReadLine());
        serial.Write("c");
        float az = int.Parse(serial.ReadLine());
        serial.Write("d");
        float gx = int.Parse(serial.ReadLine());
        serial.Write("e");
        float gy = int.Parse(serial.ReadLine());
        serial.Write("f");
        float gz = int.Parse(serial.ReadLine());

        x += gy / 5500;
        y += gz / 5500;
        z += gx / 5500;

        transform.localEulerAngles = new Vector3(x, -y, -z);

        /* Stick */

        float angY = aStick.localEulerAngles.y;

        if (angY != 0)
            aStick.Rotate(0f, -angY, 0f);

        if (Input.GetKeyDown(KeyCode.W))
            aStick.Rotate(10f, 0f, 0f);
        else if (Input.GetKeyUp(KeyCode.W))
            aStick.Rotate(-10f, 0f, 0f);
        
        if (Input.GetKeyDown(KeyCode.S))
            aStick.Rotate(-10f, 0f, 0f);
        else if (Input.GetKeyUp(KeyCode.S))
            aStick.Rotate(10f, 0f, 0f);

        if (Input.GetKeyDown(KeyCode.A))
            aStick.Rotate(0f, 0f, 10f);
        else if (Input.GetKeyUp(KeyCode.A))
            aStick.Rotate(0f, 0f, -10f);

        if (Input.GetKeyDown(KeyCode.D))
            aStick.Rotate(0f, 0f, -10f);
        else if (Input.GetKeyUp(KeyCode.D))
            aStick.Rotate(0f, 0f, 10f);

        /* Buttons */

        if (Input.GetKeyDown(KeyCode.T))
            button0.material.color = new Color(1f, 0f, 0f, 1f);
        else if (Input.GetKeyUp(KeyCode.T))
            button0.material.color = defaultColor;

        if (Input.GetKeyDown(KeyCode.H))
            button1.material.color = new Color(1f, 0f, 0f, 1f);
        else if (Input.GetKeyUp(KeyCode.H))
            button1.material.color = defaultColor;

        if (Input.GetKeyDown(KeyCode.F))
            button2.material.color = new Color(1f, 0f, 0f, 1f);
        else if (Input.GetKeyUp(KeyCode.F))
            button2.material.color = defaultColor;

        if (Input.GetKeyDown(KeyCode.G))
            button3.material.color = new Color(1f, 0f, 0f, 1f);
        else if (Input.GetKeyUp(KeyCode.G))
            button3.material.color = defaultColor;

        if (Input.GetKeyDown(KeyCode.Alpha1))
            button4.material.color = new Color(1f, 0f, 0f, 1f);
        else if (Input.GetKeyUp(KeyCode.Alpha1))
            button4.material.color = defaultColor;

        if (Input.GetKeyDown(KeyCode.Alpha2))
            button5.material.color = new Color(1f, 0f, 0f, 1f);
        else if (Input.GetKeyUp(KeyCode.Alpha2))
            button5.material.color = defaultColor;

        if (Input.GetKeyDown(KeyCode.Alpha3))
            stickButton.material.color = new Color(1f, 0f, 0f, 1f);
        else if (Input.GetKeyUp(KeyCode.Alpha3))
            stickButton.material.color = defaultColor;
    }
}
