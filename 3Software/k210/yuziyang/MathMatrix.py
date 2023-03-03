class Vector:
    def __init__(self, lis):
        self._values = lis   # 在vector中设置私有变量values存储数组数据

    # 返回 dim 维零向量
    @classmethod
    def zero(cls, dim):
        return cls([0] * dim)

    # 返回向量的模
    def norm(self):
        return math.sqrt(sum(e**2 for e in self))

    # 返回向量的单位向量
    def normalize(self):
        if self.norm() < 1e-8:
            raise ZeroDivisionError("Normalize error! norm is zero.")
        return 1 / self.norm() * Vector(self._values)

    # 向量相加，返回结果向量
    def __add__(self, other):
        assert len(self) == len(other), "error in adding,length of vectors must be same"
        return Vector([a + b for a, b in zip(self, other)])

    # 向量相减，返回结果向量
    def __sub__(self, other):
        assert len(self) == len(other), "error in subbing,length of vectors must be same"
        return Vector([a - b for a, b in zip(self, other)])

    # 向量点乘，返回结果向量
    def dot(self, other):
        assert len(self) == len(other),\
            "error in doting,length of vectors must be same."
        return sum(a * b for a,b in zip(self, other))

    # 向量左乘标量，返回结果向量
    def __mul__(self, k):
        return Vector([a * k for a in self])

    # 向量右乘标量，返回结果向量
    def __rmul__(self, k):
        return Vector([a * k for a in self])

    # 向量数量除法，返回结果向量
    def __truediv__(self, k):
        return 1/k * self

    # 向量取正
    def __pos__(self):
        return 1*self

    # 向量取负
    def __neg__(self):
        return -1*self

    # 取出向量的第index个元素,调用时直接vec[]
    def __getitem__(self, index):
        return self._values[index]

    # 返回向量长度，调用时直接len(vec)
    def __len__(self):
        return len(self._values)

    # 返回向量Vector(...)
    def __repr__(self):  # __repr__和__str__都在调用类时自动执行其中一个，倾向位置在最后一个
        return "Vector({})".format(self._values)

    # 返回向量(...),调用时直接print(vec)
    def __str__(self):
        return "({})".format(",".join(str(e) for e in self._values))
class Matrix:

    def __init__(self, list2d):
        self._values = [row[:] for row in list2d]

    @classmethod
    def zero(cls, r, c):
        """返回一个r行c列的零矩阵"""
        return cls([[0] * c for _ in range(r)])

    def T(self):
        """返回矩阵的转置矩阵"""
        return Matrix([[e for e in self.col_vector(i)]
                       for i in range(self.col_num())])

    def __add__(self, another):
        """返回两个矩阵的加法结果"""
        assert self.shape() == another.shape(), \
            "Error in adding. Shape of matrix must be same."
        return Matrix([[a + b for a, b in zip(self.row_vector(i), another.row_vector(i))]
                       for i in range(self.row_num())])

    def __sub__(self, another):
        """返回两个矩阵的减法结果"""
        assert self.shape() == another.shape(), \
            "Error in subtracting. Shape of matrix must be same."
        return Matrix([[a - b for a, b in zip(self.row_vector(i), another.row_vector(i))]
                       for i in range(self.row_num())])

    def dot(self, another):
        """返回矩阵乘法的结果"""
        if isinstance(another, Vector):
            # 矩阵和向量的乘法
            assert self.col_num() == len(another), \
                "Error in Matrix-Vector Multiplication."
            return Vector([self.row_vector(i).dot(another) for i in range(self.row_num())])

        if isinstance(another, Matrix):
            # 矩阵和矩阵的乘法
            assert self.col_num() == another.row_num(), \
                "Error in Matrix-Matrix Multiplication."
            return Matrix([[self.row_vector(i).dot(another.col_vector(j)) for j in range(another.col_num())]
                           for i in range(self.row_num())])

    def __mul__(self, k):
        """返回矩阵的数量乘结果: self * k"""
        return Matrix([[e * k for e in self.row_vector(i)]
                       for i in range(self.row_num())])

    def __rmul__(self, k):
        """返回矩阵的数量乘结果: k * self"""
        return self * k

    def __truediv__(self, k):
        """返回数量除法的结果矩阵：self / k"""
        return (1 / k) * self

    def __pos__(self):
        """返回矩阵取正的结果"""
        return 1 * self

    def __neg__(self):
        """返回矩阵取负的结果"""
        return -1 * self

    def row_vector(self, index):
        """返回矩阵的第index个行向量"""
        return Vector(self._values[index])

    def col_vector(self, index):
        """返回矩阵的第index个列向量"""
        return Vector([row[index] for row in self._values])

    def __getitem__(self, pos):
        """返回矩阵pos位置的元素"""
        r, c = pos
        return self._values[r][c]

    def ni_matrix(self):
        """返回矩阵的逆（二维矩阵）"""
        a=self._values[0][0]
        b=self._values[0][1]
        c=self._values[1][0]
        d=self._values[1][1]
        ni1 = Matrix(([d,-b],[-c,a]))
        k = 1/(a*d-b*c)
        return Matrix([[e * k for e in ni1.row_vector(i)]
                       for i in range(ni1.row_num())])

    def size(self):
        """返回矩阵的元素个数"""
        r, c = self.shape()
        return r * c

    def row_num(self):
        """返回矩阵的行数"""
        return self.shape()[0]

    __len__ = row_num

    def col_num(self):
        """返回矩阵的列数"""
        return self.shape()[1]

    def shape(self):
        """返回矩阵的形状: (行数， 列数)"""
        return len(self._values), len(self._values[0])

    def __repr__(self):
        return "Matrix({})".format(self._values)

    __str__ = __repr__
