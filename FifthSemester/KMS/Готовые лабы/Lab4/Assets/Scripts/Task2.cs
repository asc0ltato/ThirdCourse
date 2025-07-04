using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Task2 : MonoBehaviour {

    [SerializeField] Button thirdButton; 
    [SerializeField] Text c;  

    void Start()
    {
        thirdButton.interactable = false;
    }

    void Update()
    {
        if (AllFieldsFilled())
        {
            thirdButton.interactable = true;
        }
        else
        {
            thirdButton.interactable = false;
        }
    }

    bool AllFieldsFilled()
    {
        return !string.IsNullOrEmpty(c.text) && c.text != "-";
    }
}