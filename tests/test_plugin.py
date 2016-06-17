import unittest
import or_plugin
import openravepy


class Test(unittest.TestCase):
    def setUp(self):
        # Create an environment with a box.
        self.env = openravepy.Environment()
        with self.env:
            body = openravepy.RaveCreateKinBody(self.env, '')
            body.SetName('test_box')
            body.InitFromBoxes([[0, 0, 0, 0.1, 0.2, 0.3]], True)
            self.env.AddKinBody(body)

        TestModule = or_plugin.Module('or_plugin_test')
        self.test_module = TestModule(self.env)

    def tearDown(self):
        self.env.Destroy()

    def test_success(self):
        self.test_module.Success()

    def test_failure(self):
        with self.assertRaises(or_plugin.CommandError):
            self.test_module.Failure()

    def test_kinbody(self):
        kinbody = self.env.GetBodies()[0]
        output = self.test_module.Kinbody(kinbody)
        self.assertEquals(kinbody, output)

    def test_link(self):
        link = self.env.GetBodies()[0].GetLinks()[0]
        output = self.test_module.Link(link)
        self.assertEquals(link, output)

    def test_transform(self):
        transform = self.env.GetBodies()[0].GetTransform()
        output = self.test_module.Transform(transform)
        self.assertEquals(transform, output)


if __name__ == '__main__':
    unittest.main()
