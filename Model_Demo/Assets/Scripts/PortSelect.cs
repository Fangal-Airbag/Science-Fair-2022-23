using System.Collections;
using System.Collections.Generic;
using System.IO.Ports;
using UnityEngine;
using UnityEngine.SceneManagement;
using TMPro;

public class PortSelect : MonoBehaviour
{
    public TMP_Dropdown dropdown;

    public string selected;

    void Start()
    {
        dropdown.ClearOptions();

        List<string> options = new List<string>();

        foreach (string s in SerialPort.GetPortNames()) {
            string option = s;
            options.Add(option);
        }

        dropdown.AddOptions(options);
    }

    public void OnClick()
    {
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex + 1);
    }

    void Update()
    {
        selected = dropdown.options[dropdown.value].text;
        PlayerPrefs.SetString("portSelected", selected);
    }
}
