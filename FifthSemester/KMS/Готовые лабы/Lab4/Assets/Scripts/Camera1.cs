using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Camera1 : MonoBehaviour {
    [SerializeField]
    GameObject potolok;
    [SerializeField]
    GameObject pol;
    [SerializeField]
    GameObject stenaSleva;
    [SerializeField]
    GameObject stenaSprava;
    [SerializeField]
    GameObject stenaSzadi;
    [SerializeField]
    GameObject stenaSperedi;
    [SerializeField]
    GameObject stol; 

    [SerializeField]
    Transform centralObject;  
    [SerializeField]
    float sensivity = 5f;  
    [SerializeField]
    float scrollSpeed = 5f;  
    [SerializeField]
    float maxdistance = 8.5f;  
    [SerializeField]
    float mindistance = 1f;
    [SerializeField]
    float collisionOffset = 0.5f;

    private float verticalRotation = 0f;
    private float maxVerticalAngle = 60f;

    Vector3 Position(Vector3 position)
    {
        float X = Mathf.Clamp(position.x, stenaSleva.transform.position.x , stenaSprava.transform.position.x);
        float Y = Mathf.Clamp(position.y, pol.transform.position.y, potolok.transform.position.y );
        float Z = Mathf.Clamp(position.z, stenaSzadi.transform.position.z , stenaSperedi.transform.position.z );

        return new Vector3(X, Y, Z);
    }

    Vector3 Collisions(Vector3 newPosition)
    {
        RaycastHit hit;

        if (Physics.Raycast(transform.position, newPosition - transform.position, out hit, collisionOffset))
        {
            if (hit.transform.gameObject == stol)
            {
                return transform.position;
            }
        }
        return newPosition;
    }

    void Update()
    {
        if (Input.GetMouseButton(1)) 
        {
            float y = Input.GetAxis("Mouse X") * sensivity;
            if (y != 0) transform.RotateAround(centralObject.position, Vector3.up, y);
        }

        if (Input.GetMouseButton(0))
        {
            float v = -Input.GetAxis("Mouse Y") * sensivity;
            verticalRotation = Mathf.Clamp(verticalRotation + v, -maxVerticalAngle, maxVerticalAngle);
            transform.localEulerAngles = new Vector3(verticalRotation, transform.localEulerAngles.y, 0);
        }

        float x = Input.GetAxis("Horizontal") / sensivity;    
        if (x != 0)
        {
            Vector3 newpos = transform.position + transform.TransformDirection(new Vector3(x, 0, 0));
            newpos = Collisions(newpos);
            if (ControlDistance(Vector3.Distance(newpos, centralObject.position)))
            {
                transform.position = Position(newpos);
            }
        }

        float z = Input.GetAxis("Vertical") / sensivity;
        if (z != 0)
        {
            Vector3 newpos = transform.position + transform.TransformDirection(new Vector3(0, 0, z));
            newpos = Collisions(newpos);
            if (ControlDistance(Vector3.Distance(newpos, centralObject.position)))
            {
                transform.position = Position(newpos);
            }
        }

        float scroll = Input.GetAxis("Mouse ScrollWheel") * scrollSpeed;
        if (scroll != 0)
        {
            Vector3 newpos = transform.position + transform.TransformDirection(Vector3.forward * scroll);
            newpos = Collisions(newpos);
            if (ControlDistance(Vector3.Distance(newpos, centralObject.position)))
            {
                transform.position = Position(newpos);
            }
        }
    }

    bool ControlDistance(float distance)
    {
        if (distance > mindistance && distance < maxdistance) return true;
        return false;
    }
}