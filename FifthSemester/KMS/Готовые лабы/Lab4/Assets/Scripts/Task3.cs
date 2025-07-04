using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Task3 : MonoBehaviour {

    public Animator anim1;
    public Animator anim2;

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.E))
        {
            anim1.SetTrigger("hittenOff");
        }

        if (Input.GetKeyDown(KeyCode.T))
        {
            anim2.SetTrigger("hittenOff");
        }

    }
}
