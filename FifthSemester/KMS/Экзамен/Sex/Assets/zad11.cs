using UnityEngine;

public class zad11 : MonoBehaviour {
    private Renderer rend;
    float speed = 0.1f;

    void Start()
    {
        rend = GetComponent<Renderer>();
    }
    
    void Update () {
        float x = Input.GetAxis("Horizontal") * speed;
        float z = Input.GetAxis("Vertical") * speed;
        transform.Translate(x, 0, z);
    }

    void OnCollisionEnter(Collision col)
    {
        if (col.gameObject.name == "Capsule2")
        {
            rend.material.color = Color.red;
            Renderer otherRend = col.gameObject.GetComponent<Renderer>();
            otherRend.material.color = Color.green;
        }
    }
}